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

WorkTracker::WorkTracker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WorkTracker)
    , m_isRecording(false)
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

    m_statusDay       = new QLabel(this);
    m_statusRecording = new QLabel(this);
    ui->statusBar->addWidget(m_statusDay, 1);
    ui->statusBar->addWidget(m_statusRecording, 1);

    this->resize(this->width(), this->height() - ui->frame->height()
                                               - ui->webView->height());

    QFile xmlFile("D:/RobertLohr/Programmieren/Cpp/WorkTracker/WorkTrackerDatabase.xml");
    if (!m_xml.setContent(&xmlFile)) {
        qDebug() << "Unable to load database";
    }
    xmlFile.close();

    m_tasks.setDataSource(&m_xml);
    m_workdays.setDataSource(&m_xml);
    // For the completer
    m_taskModel = new TaskListModel(&m_tasks, this);

    QCompleter *completer = new QCompleter(this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    completer->setModel(m_taskModel);
    ui->tasksEdit->setCompleter(completer);

    connect(ui->selectTaskButton, SIGNAL(clicked()),  this, SLOT(taskSelected()));
    connect(ui->taskButton,       SIGNAL(clicked()),  this, SLOT(toggleTask()));
    connect(ui->workdayButton,    SIGNAL(clicked()),  this, SLOT(startWorkday()));
    connect(ui->summaryButton,    SIGNAL(clicked()),  this, SLOT(showSummary()));
    connect(&showAnimation,       SIGNAL(finished()), this, SLOT(showAnimationFinished()));
}

WorkTracker::~WorkTracker()
{
    delete ui;
    QFile xmlFile("D:/RobertLohr/Programmieren/Cpp/WorkTracker/WorkTrackerDatabaseOut.xml");
    xmlFile.open(QIODevice::WriteOnly);

    QTextStream out(&xmlFile);
    out.setCodec("UTF-8");

    QDomNode xmlNode = m_xml.createProcessingInstruction(
                "xml", "version=\"1.0\" encoding=\"UTF-8\"");
    m_xml.insertBefore(xmlNode, m_xml.firstChild());
    m_xml.save(out, 2, QDomNode::EncodingFromTextStream);
    xmlFile.close();
}

void
WorkTracker::taskSelected()
{
    toggleInput();

    QString taskName = ui->tasksEdit->text();

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
    }
}

void
WorkTracker::showAnimationFinished()
{
    // The frame can only be shown once there is enough room for it. Otherwise Qt would
    // just create space for it on its own and the animation would add more space on top
    // of that, resulting in a too large window.
    ui->frame->setVisible(true);
}

void
WorkTracker::startWorkday()
{
    if (!m_workday.isNull()) {
        m_workdays.addWorkDay(m_workday);
    }
    m_workday.clear();
    m_workday.setDay(QDate::currentDate());

    //ui->statusBar->removeWidget();

    QString date = QDateTime::currentDateTime().toString(Qt::TextDate);
    m_statusDay->setText(tr("Workday started at %1").arg(date));
}

void
WorkTracker::showSummary()
{
    QString html = m_workday.generateSummary();
    ui->webView->setHtml(html);
    ui->webView->setVisible(true);
}
