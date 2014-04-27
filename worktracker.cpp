#include "worktracker.h"
#include "ui_worktracker.h"
#include "worktask.h"

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QCompleter>
#include <QPropertyAnimation>
#include <QSize>
#include <QTime>
#include <QDateTime>
#include <QLabel>
#include <QWebView>
#include <QWebFrame>
#include <QDir>

WorkTracker::WorkTracker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WorkTracker)
    , m_isRecording(false)
    , m_isNewWorkDay(false)
{
    ui->setupUi(this);

    showAnimation.setTargetObject(this);
    showAnimation.setPropertyName("size");
    showAnimation.setDuration(200);

    hideAnimation.setTargetObject(this);
    hideAnimation.setPropertyName("size");
    hideAnimation.setDuration(200);

    ui->frame->setVisible(false);
    ui->webView->setVisible(false);

    m_statusDay       = new QLabel(tr("Waiting to start a new workday"), this);
    m_statusRecording = new QLabel(tr("Not recording anything yet"), this);
    ui->statusBar->addWidget(m_statusDay, 1);
    ui->statusBar->addWidget(m_statusRecording, 1);

    this->resize(this->width(), this->height() - ui->frame->height()
                                               - ui->webView->height());

    QFile xmlFile(QDir::currentPath() + "/WorkTrackerDatabase.xml");
    if (!m_xml.setContent(&xmlFile)) {
        qDebug() << "Unable to load database, create empty";

        QDomNode xmlNode = m_xml.createProcessingInstruction(
                    "xml", "version=\"1.0\" encoding=\"UTF-8\"");
        m_xml.appendChild(xmlNode);

        QDomElement root = m_xml.createElement("worktracker");
        m_xml.appendChild(root);

        QDomElement version = m_xml.createElement("version");
        version.appendChild(m_xml.createTextNode("1.0"));
        root.appendChild(version);

        root.appendChild(m_xml.createElement("tasks"));
        root.appendChild(m_xml.createElement("workdays"));
    }
    xmlFile.close();

    m_tasks.setDataSource(&m_xml);
    m_workdays.setDataSource(&m_xml);
    m_workday = m_workdays.findToday();
    if (!m_workday.isNull()) {
        toggleWorkday();
    }

    // For the completer
    m_taskModel = new TaskListModel(&m_tasks, this);

    QCompleter *completer = new QCompleter(this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    completer->setModel(m_taskModel);
    ui->tasksEdit->setCompleter(completer);

    connect(ui->selectTaskButton, SIGNAL(clicked()),  this, SLOT(taskSelected()));
    connect(ui->taskButton,       SIGNAL(clicked()),  this, SLOT(toggleTask()));
    connect(ui->workdayButton,    SIGNAL(clicked()),  this, SLOT(toggleWorkday()));
    connect(ui->summaryButton,    SIGNAL(clicked()),  this, SLOT(showSummary()));
    connect(&showAnimation,       SIGNAL(finished()), this, SLOT(showAnimationFinished()));
}

WorkTracker::~WorkTracker()
{
    delete ui;

    if (!m_workday.isNull()) {
        m_workdays.addWorkDay(m_workday);
    }

    QFile xmlFile(QDir::currentPath() + "/WorkTrackerDatabase.xml");
    xmlFile.open(QIODevice::WriteOnly);

    QTextStream out(&xmlFile);
    out.setCodec("UTF-8");
    m_xml.save(out, 2, QDomNode::EncodingFromTextStream);
    xmlFile.close();
}

void
WorkTracker::taskSelected()
{
    QString taskName = ui->tasksEdit->text();
    if (taskName.isEmpty()) {
        ui->statusBar->showMessage(tr("You must enter a task description"), 3000);
        return;
    }

    toggleInput();

    m_taskModel->appendItem(taskName);
    ui->taskButton->setEnabled(true);
    ui->summaryButton->setEnabled(true);

    Task task = m_tasks.itemByName(taskName);
    m_workday.addTask(WorkTask(task, m_taskStart, QTime::currentTime()));
}

void
WorkTracker::toggleTask()
{
    if (m_isRecording) {
        ui->taskButton->setText(tr("Start &Task"));
        ui->taskButton->setEnabled(false);
        ui->summaryButton->setEnabled(false);
        toggleInput();

        m_statusRecording->setText(tr("Task stopped at %1")
                                   .arg(m_taskStart.toString(Qt::TextDate)));
    }
    else {
        hideSummary();
        ui->workdayButton->setEnabled(false);

        ui->taskButton->setText(tr("Stop &Task"));
        m_taskStart = QTime::currentTime();

        m_statusRecording->setText(tr("Task started at %1")
                                   .arg(m_taskStart.toString(Qt::TextDate)));
    }

    m_isRecording = !m_isRecording;
}

void
WorkTracker::toggleInput()
{
    int height = ui->frame->height();

    if (!m_isRecording) {
        ui->workdayButton->setEnabled(true);

        // Hide the frame and shrink the window
        ui->frame->setVisible(false);

        QSize size = this->size();
        hideAnimation.setStartValue(size);

        size.setHeight(size.height() - height);
        hideAnimation.setEndValue(size);
        hideAnimation.start();
    }
    else {
        // Expand the window so there's room to display the frame for the task input.
        QSize size = this->size();
        showAnimation.setStartValue(size);

        size.setHeight(size.height() + height);
        showAnimation.setEndValue(size);
        showAnimation.start();
        m_animatedWidget = ui->frame;
    }
}

void
WorkTracker::showAnimationFinished()
{
    // The frame can only be shown once there is enough room for it. Otherwise Qt would
    // just create space for it on its own and the animation would add more space on top
    // of that, resulting in a too large window.
    m_animatedWidget->setVisible(true);
}

void
WorkTracker::toggleWorkday()
{
    QString date = QDateTime::currentDateTime().toString(Qt::TextDate);

    if (!m_isNewWorkDay) {
        // Start Workday was clicked
        hideSummary();

        m_workday.setDay(QDate::currentDate());

        m_statusDay->setText(tr("Work started at %1").arg(date));

        ui->workdayButton->setText(tr("Stop Workday"));
        ui->taskButton->setEnabled(true);
    }
    else {
        // Stop Workday was clicked
        m_statusDay->setText(tr("Work stopped at %1").arg(date));

        ui->workdayButton->setText(tr("Start New Workday"));
        ui->taskButton->setEnabled(false);
        ui->summaryButton->setEnabled(!m_workday.isNull() && !ui->webView->isVisible());

        if (!m_workday.isNull()) {
            m_workdays.addWorkDay(m_workday);
            m_workday.clear();
        }
    }

    m_isNewWorkDay = !m_isNewWorkDay;
}

void
WorkTracker::showSummary()
{
    ui->summaryButton->setEnabled(false);

    if (!ui->webView->isVisible()) {
        QString html = m_workday.generateSummary();
        ui->webView->setHtml(html);

        QSize size = this->size();
        showAnimation.setStartValue(size);

        int height = ui->webView->page()->mainFrame()->contentsSize().height();
        size.setHeight(size.height() + height);
        showAnimation.setEndValue(size);
        showAnimation.start();
        m_animatedWidget = ui->webView;
    }
}

void
WorkTracker::hideSummary()
{
    ui->webView->setVisible(false);

    QSize size = this->size();
    hideAnimation.setStartValue(size);

    size.setHeight(size.height() - ui->webView->height());
    hideAnimation.setEndValue(size);
    hideAnimation.start();
}
