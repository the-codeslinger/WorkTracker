#include "worktracker.h"
#include "controller/worktrackercontroller.h"

#include <QApplication>
#include <QStandardPaths>
#include <QString>
#include <QDebug>
#include <QDomDocument>
#include <QDomNode>
#include <QDomElement>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Typical Nerd");
    a.setApplicationName("WorkTracker");

    // Set up the data source for our application, i.e. load an existing database or, e.g.
    // on the first start, create a new database.
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    qDebug() << "Write database to " << dataPath;

    QDir dir(dataPath);
    if (!dir.exists()) {
        if (!dir.mkpath(dataPath)) {
            qDebug() << "Cannot create dir " << dataPath << ". Database cannot be saved";
        }
    }

    QFile xmlFile(dataPath + "/Database.xml");

    QDomDocument dataSource;
    if (!dataSource.setContent(&xmlFile)) {
        qDebug() << "No database file found; Create new";

        QString procInstruction = "version=\"1.0\" encoding=\"UTF-8\"";
        QDomNode xmlNode = dataSource.createProcessingInstruction("xml", procInstruction);
        dataSource.appendChild(xmlNode);

        QDomElement root = dataSource.createElement("worktracker");
        dataSource.appendChild(root);

        QDomElement version = dataSource.createElement("version");
        version.appendChild(dataSource.createTextNode("1.0"));
        root.appendChild(version);

        root.appendChild(dataSource.createElement("tasks"));
        root.appendChild(dataSource.createElement("workdays"));
    }
    xmlFile.close();

    WorkTrackerController controller(&dataSource);
    WorkTracker w(&controller);
    controller.setUi(&w);
    controller.run();

    int result = a.exec();

    // Save the database now that the application is finished
    if (!xmlFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open database for writing; Save current data not possible";
    }
    else {
        QTextStream out(&xmlFile);
        out.setCodec("UTF-8");
        dataSource.save(out, 2, QDomNode::EncodingFromTextStream);
        xmlFile.close();
    }

    return result;
}
