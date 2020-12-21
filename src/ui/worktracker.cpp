/*
 * Copyright 2014 Robert Lohr
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "worktracker.h"
#include "aboutdialog.h"
#include "editorwizard.h"
#include "ui_worktracker.h"
#include "model/tasklistmodel.h"
#include "../model/worktask.h"
#include "../model/task.h"
#include "../controller/worktrackercontroller.h"
#include "../controller/editorcontroller.h"

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QCompleter>
#include <QPropertyAnimation>
#include <QSize>
#include <QTime>
#include <QDateTime>
#include <QLabel>
#include <QTextEdit>
#include <QDir>
#include <QIcon>
#include <QFontMetrics>
#include <QShowEvent>
#include <QTranslator>
#include <QSettings>
#include <QEvent>

class CompleteEvent : public QEvent
{
public:
    CompleteEvent(QLineEdit* lineEdit)
        : QEvent(QEvent::User)
        , m_lineEdit(lineEdit)
    { }

    void complete()
    {
        if (nullptr != m_lineEdit) {
            m_lineEdit->completer()->complete();
        }
    }

private:
    QLineEdit* m_lineEdit;
};

WorkTracker::WorkTracker(WorkTrackerController* controller, QWidget *parent)
    : QMainWindow{parent}
    , ui{new Ui::WorkTracker}
    , m_controller{controller}
    , m_collapsedHeight{0}
    , m_hours{0}
    , m_minutes{0}
{
    ui->setupUi(this);

    QSettings settings;
    restoreGeometry(settings.value("Geometry").toByteArray());

    setupLanguageMenu();
    loadTranslations();

    // m_controller->load() is called in showEvent(QShowEvent*) to have the ui ready to
    // be able to correctly elide text in the status bar.

    m_showAnimation.setTargetObject(this);
    m_showAnimation.setPropertyName("size");
    m_showAnimation.setDuration(200);

    m_hideAnimation.setTargetObject(this);
    m_hideAnimation.setPropertyName("size");
    m_hideAnimation.setDuration(200);

    ui->frame->setVisible(false);
    ui->textEdit->setVisible(false);

    m_statusRecording = new QLabel{this};
    m_statusDuration  = new QLabel{this};
    ui->statusBar->addWidget(m_statusRecording, 1);
    ui->statusBar->addWidget(m_statusDuration, 0);
    translate();

    // Capture the current width which is set by the designer. Then we resize the window
    // to only take up as much space as is needed with the edit field and text edit not
    // being visible. Since this also changes the width we restore it using the value
    // saved earliert. Voila, our UI looks correct.
    auto width = this->width();
    this->adjustSize();
    this->resize(width, this->height());
    m_collapsedHeight = this->height();

    auto completer = new QCompleter{this};
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    
    ui->tasksEdit->setCompleter(completer);

    ui->tasksEdit->setClearButtonEnabled(true);

    connect(ui->workdayButton,    SIGNAL(clicked()),  
            m_controller,         SLOT(toggleWorkDay()));
    connect(ui->selectTaskButton, SIGNAL(clicked()),
            this,                 SLOT(taskSelected()));
    connect(ui->taskButton,       SIGNAL(clicked()),
            this,                 SLOT(showInput()));
    connect(ui->summaryButton,    SIGNAL(clicked()),
            this,                 SLOT(showSummary()));
    connect(&m_showAnimation,     SIGNAL(finished()),
            this,                 SLOT(showAnimationFinished()));
    connect(ui->tasksEdit,        SIGNAL(returnPressed()),
            ui->selectTaskButton, SIGNAL(clicked()));

    connect(m_controller,         SIGNAL(workDayStarted(QDateTime)),
            this,                 SLOT(workDayStarted(QDateTime)));
    connect(m_controller,         SIGNAL(workDayStopped(QDateTime)),
            this,                 SLOT(workDayStopped(QDateTime)));
    connect(m_controller,         SIGNAL(workTaskStarted(QDateTime, QString)),
            this,                 SLOT(workTaskStarted(QDateTime, QString)));
    connect(m_controller,         SIGNAL(workTaskStopped(QDateTime, QString)),
            this,                 SLOT(workTaskStopped(QDateTime, QString)));
    connect(m_controller,         SIGNAL(totalTimeChanged(int, int)),
            this,                 SLOT(totalTimeChanged(int, int)));

    // Menu
    connect(ui->actionQuit,     SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionAbout_QT, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionAbout,    SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionEditor,   SIGNAL(triggered()), this, SLOT(showEditor()));
}

WorkTracker::~WorkTracker()
{
    delete ui;
}

void
WorkTracker::workDayStarted(const QDateTime& /* now */)
{
    hideSummary();

    ui->workdayButton->setText(tr("Stop &Workday"));
    ui->workdayButton->setIcon(QIcon{":/icon/Stop-Day.svg"});
    ui->taskButton->setEnabled(true);
}

void
WorkTracker::workDayStopped(const QDateTime& /* now */)
{
    ui->workdayButton->setText(tr("Start &New Workday"));
    ui->workdayButton->setIcon(QIcon{":/icon/Start-Day.svg"});
    ui->taskButton->setEnabled(false);
}

void
WorkTracker::workTaskStarted(const QDateTime& now, const QString& name)
{
    auto dateString = now.toLocalTime().toString(Qt::TextDate);
    
    m_name = name;
    m_timestamp = now;

    hideSummary();
    ui->workdayButton->setEnabled(false);
    ui->taskButton->setEnabled(true);
    ui->taskButton->setText(tr("Stop &Task"));
    ui->taskButton->setIcon(QIcon{":/icon/Stop-Task.svg"});
    ui->tasksEdit->setText(name); // After restart this would be empty
    setShortenedTaskStatusText(tr("%1 started at %2").arg(name, dateString));
}

void
WorkTracker::workTaskStopped(const QDateTime& now, const QString& name)
{
    auto dateString = now.toLocalTime().toString(Qt::TextDate);
    
    m_name = name;
    m_timestamp = now;

    hideSummary();
    ui->workdayButton->setEnabled(true);
    ui->taskButton->setText(tr("Start &Task"));
    ui->taskButton->setIcon(QIcon{":/icon/Start-Task.svg"});

    setShortenedTaskStatusText(tr("%1 stopped at %2").arg(name, dateString));
}

void
WorkTracker::taskSelected()
{
    auto taskName = ui->tasksEdit->text();
    if (taskName.isEmpty()) {
        ui->statusBar->showMessage(tr("You must enter a task description"), 3000);
        return;
    }

    m_controller->toggleTask(taskName);

    ui->taskButton->setEnabled(true);
    ui->summaryButton->setEnabled(true);

    hideInput();
}

void
WorkTracker::showInput()
{
    ui->textEdit->setVisible(false);
    ui->taskButton->setEnabled(false);
    ui->summaryButton->setEnabled(false);

    auto size = this->size();
    m_showAnimation.setStartValue(size);

    size.setHeight(m_collapsedHeight + ui->frame->height());
    m_showAnimation.setEndValue(size);
    m_showAnimation.start();
    m_animatedWidget = ui->frame;

    // Create a new model every time so that we always have the most current data to 
    // work with. The completer's filter string (completion prefix) is also reset in 
    // order to have the whole list to choose from. This is supposed to save typing.
    auto completer = ui->tasksEdit->completer();
    auto model     = new TaskListModel{m_controller->dataSource(), completer};
    
    completer->setModel(model);
    completer->setCompletionPrefix("");
}

void
WorkTracker::hideInput()
{
    // Hide the frame and shrink the window
    ui->frame->setVisible(false);

    auto size = this->size();
    m_hideAnimation.setStartValue(size);

    size.setHeight(m_collapsedHeight);
    m_hideAnimation.setEndValue(size);
    m_hideAnimation.start();
}

void
WorkTracker::showSummary()
{
    if (!ui->textEdit->isVisible()) {
        ui->frame->setVisible(false);

        auto html = m_controller->generateSummary();
        ui->textEdit->setHtml(html);

        auto size = this->size();
        m_showAnimation.setStartValue(size);

        auto contentHeight = ui->textEdit->document()->size();
        size.setHeight(m_collapsedHeight + contentHeight.height());
        m_showAnimation.setEndValue(size);
        m_showAnimation.start();
        m_animatedWidget = ui->textEdit;

        ui->summaryButton->setText(tr("Hide Summary"));
    }
    else {
        hideSummary();
    }
}

void
WorkTracker::hideSummary()
{
    if (ui->textEdit->isVisible()) {
        ui->textEdit->setVisible(false);

        auto size = this->size();
        m_hideAnimation.setStartValue(size);

        size.setHeight(m_collapsedHeight);
        m_hideAnimation.setEndValue(size);
        m_hideAnimation.start();
    }

    ui->summaryButton->setText(tr("Summary"));
}

void
WorkTracker::showAnimationFinished()
{
    // The frame can only be shown once there is enough room for it. Otherwise Qt would
    // just create space for it on its own and the animation would add more space on top
    // of that, resulting in a too large window.
    m_animatedWidget->setVisible(true);
    if (m_animatedWidget == ui->frame) {
        ui->tasksEdit->setFocus();
        ui->tasksEdit->completer()->complete();
    }
}

void
WorkTracker::about()
{
    AboutDialog dlg;
    dlg.exec();
}

void
WorkTracker::showEditor()
{
    EditorController econ(m_controller->dataSource(), this);
    EditorWizard wizard(&econ, this);
    
    connect(&econ,        &EditorController::currentTaskClosed,
            m_controller, &WorkTrackerController::closeCurrentTask);
    connect(&econ,        &EditorController::activeTaskChanged,
            m_controller, &WorkTrackerController::setActiveTask);
    
    wizard.exec();
}

void
WorkTracker::setShortenedTaskStatusText(const QString& text) const
{
    auto maxWidth     = m_statusRecording->width();
    auto metrics      = m_statusRecording->fontMetrics();
    auto elidedString = metrics.elidedText(text, Qt::ElideMiddle, maxWidth);

    m_statusRecording->setText(elidedString);
}

void
WorkTracker::totalTimeChanged(int hours, int minutes)
{
    m_statusDuration->setText(tr("Total time %1h %2m").arg(hours).arg(minutes));
    m_hours   = hours;
    m_minutes = minutes;
}

void 
WorkTracker::changeEvent(QEvent* p_event)
{
    if (QEvent::LanguageChange == p_event->type()) {
        ui->retranslateUi(this);
        translate();
    }
    
    QMainWindow::changeEvent(p_event);
}

void
WorkTracker::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);
    if (!event->spontaneous()) {
        if (!m_controller->load()) {
            // TODO: Do something to show a message to the user.
        }
    }
}

void 
WorkTracker::translate()
{
    auto dateString = m_timestamp.toLocalTime().toString(Qt::TextDate);
    
    if (m_controller->isRecording()) {
        ui->taskButton->setText(tr("Stop &Task"));
        
        if (!m_name.isEmpty()) {
            setShortenedTaskStatusText(tr("%1 started at %2").arg(m_name, dateString));
        }
    }
    else {
        ui->taskButton->setText(tr("Start &Task"));
        
        if (!m_name.isEmpty()) {
            setShortenedTaskStatusText(tr("%1 stopped at %2").arg(m_name, dateString));
        }
    }
    
    if (m_controller->isActiveDay()) {
        ui->workdayButton->setText(tr("Stop &Workday"));
    }
    else {
        ui->workdayButton->setText(tr("Start &New Workday"));
        m_statusRecording->setText("");
    }
    
    totalTimeChanged(m_hours, m_minutes);
}

void 
WorkTracker::setupLanguageMenu()
{
    ui->actionEnUS->setData(QVariant{"en_US"});
    ui->actionDeDE->setData(QVariant{"de_DE"});
    
    connect(ui->actionEnUS, &QAction::triggered, this, &WorkTracker::languageSelected);
    connect(ui->actionDeDE, &QAction::triggered, this, &WorkTracker::languageSelected);
    connect(this, &WorkTracker::languageChanged, this, &WorkTracker::setLanguage);
}

void 
WorkTracker::loadTranslations()
{
    // These are the languages the application supports
    auto locales = QVector<QString>{ "en_US", "de_DE" };
    
#if defined(Q_OS_LINUX)
    // On Linux the translations can be found in /usr/share/worktracker/l10n.
    auto l10nPath = "/../share/worktracker/l10n/";
#elif defined (Q_OS_WIN)
    // On Windows the translations are in the l10n folder in the exe dir.
    auto l10nPath = "/l10n/";
#else
    // On OS X the data is somewhere in the bundle.
#endif
    
    // Load the user settings. If none is selected (probably after an update) then try
    // the system language. If that is not supported then fall back to English.
    QSettings settings;
    auto appLocale = settings.value("Locale").toString();
    if (appLocale.isEmpty()) {
        appLocale = QLocale{}.name();
        settings.setValue("Language", appLocale);
    }
    
    if (!locales.contains(appLocale)) {
        appLocale = "en_US";
        settings.setValue("Language", appLocale);
    }
    
    // Now load the translationss and install one
    auto appDir = QApplication::applicationDirPath() + l10nPath;
    
    for (const auto& locale : locales) {
        // Load Qt language file first
        auto* qtLang = new QTranslator{this};
        if (!qtLang->load(appDir + "qt_" + locale + ".qm")) {
            qDebug() << "Cannot load Qt language file <" << locale << "> use English";
            delete qtLang;
            qtLang = nullptr;
        }
        
        // Now load WorkTracker language file
        auto* appLang = new QTranslator{this};
        if (!appLang->load(appDir + locale + ".qm")) {
            qDebug() << "Cannot load WorkTracker language file <" << locale 
                     << "> use English";
            delete appLang;
            appLang = nullptr;
        }
        
        m_translations.insert(locale, qMakePair(qtLang, appLang));
        
        if (locale == appLocale) {
            QLocale::setDefault(QLocale{appLocale});
            qApp->installTranslator(qtLang);
            qApp->installTranslator(appLang);
            m_currentLocale = appLocale;
        }
    }
    setLanguageChecked(m_currentLocale);
}

void 
WorkTracker::languageSelected()
{
    auto* source = qobject_cast<QAction*>(sender());
    if (nullptr == source) {
        qDebug() << "Sender is not a QAction. Cannot change language.";
        return;
    }
    
    auto locale = source->data().toString();
    setLanguageChecked(locale);
    emit languageChanged(locale);
}

void 
WorkTracker::setLanguageChecked(const QString& p_locale)
{
    for (auto* action : ui->menuLanguage->actions()) {
        auto locale = action->data().toString();
        action->setChecked(locale == p_locale);
    }
}

void
WorkTracker::setLanguage(const QString& p_locale)
{
    if (!m_translations.contains(p_locale)) {
        qDebug() << "Language doesn't exist <" << p_locale << ">";
        return;
    }

    auto current     = m_translations.value(m_currentLocale);
    auto translation = m_translations.value(p_locale);

    qApp->removeTranslator(current.first);
    qApp->removeTranslator(current.second);
    
    QLocale::setDefault(QLocale{p_locale});
    
    qApp->installTranslator(translation.first);
    qApp->installTranslator(translation.second);

    m_currentLocale = p_locale;
    
    QSettings settings;
    settings.setValue("Locale", p_locale);
}

void 
WorkTracker::closeEvent(QCloseEvent* event)
{
    QSettings settings;
    settings.setValue("Geometry", saveGeometry());
    QMainWindow::closeEvent(event);
}
