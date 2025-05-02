#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include "desktopmaestrobrick.h"
#include "startupbrick.h"

int main(int argc, char *argv[]) {
    // Initialize logging
    QFile logFile("/home/ares/QxCentre/QxCentre/qxcentre.log");
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qInstallMessageHandler([](QtMsgType, const QMessageLogContext &, const QString &msg) {
            QFile log("/home/ares/QxCentre/QxCentre/qxcentre.log");
            if (log.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
                QTextStream out(&log);
                out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " [DEBUG] " << msg << "\n";
            }
        });
    }
    qDebug() << "Logging initialized to qxcentre.log";

    QApplication app(argc, argv);
    qDebug() << "QApplication initialized";

    StartupBrick startup;
    qDebug() << "StartupBrick initialized";

    DesktopMaestroBrick::instance()->initializeDesktop();
    return app.exec();
}
