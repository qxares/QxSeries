#include <QApplication>
#include <QDebug>
#include <QStyleFactory>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QFile>
#include <QMessageBox>
#include <QDateTime>
#include <QTextStream>
#include <QThread>
#include <QProcess>
#include <QUuid>
#include "mainwindowbrick.h"
#include "startupbrick.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("Fusion"));
    qDebug() << "QApplication initialized";

    // Generate unique shared memory key
    QString uniqueKey = "QxCentreSharedMemory_" + QUuid::createUuid().toString();
    qDebug() << "Using unique QSharedMemory key:" << uniqueKey;

    // Clear system-level shared memory
    QProcess ipcClean;
    ipcClean.start("bash", QStringList() << "-c" << "ipcs -m | grep QxCentre | awk '{print $2}' | xargs -r ipcrm -m");
    ipcClean.waitForFinished();
    qDebug() << "Cleared system-level QSharedMemory locks:" << ipcClean.readAllStandardOutput();

    // Single-instance check
    QSystemSemaphore semaphore("QxCentreSemaphore", 1);
    QSharedMemory sharedMemory(uniqueKey);
    semaphore.acquire();
    if (sharedMemory.attach()) {
        sharedMemory.detach();
        qDebug() << "Detached stale QSharedMemory";
    }
    if (!sharedMemory.create(512, QSharedMemory::ReadWrite)) {
        QMessageBox::critical(nullptr, "QxCentre Error", "Another instance of QxCentre is already running. Please close it first.");
        qDebug() << "Another instance of QxCentre is already running";
        semaphore.release();
        return -1;
    }
    semaphore.release();

    // Set up logging
    QFile logFile("/home/ares/QxCentre/QxCentre/QxCentre.log");
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qInstallMessageHandler([](QtMsgType, const QMessageLogContext &, const QString &msg) {
            QFile file("/home/ares/QxCentre/QxCentre/QxCentre.log");
            if (file.open(QIODevice::Append)) {
                QTextStream out(&file);
                out << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << ": " << msg << "\n";
                file.close();
            }
        });
    } else {
        qDebug() << "Failed to open log file:" << logFile.errorString();
    }

    StartupBrick *startupBrick = new StartupBrick();
    if (!startupBrick->initialize()) {
        qDebug() << "Failed to initialize StartupBrick";
        delete startupBrick;
        sharedMemory.detach();
        logFile.close();
        return -1;
    }
    qDebug() << "StartupBrick initialization complete, launching MainWindowBrick";

    MainWindowBrick mainWindow;
    mainWindow.show();
    mainWindow.raise();
    for (int i = 0; i < 5; ++i) {
        QApplication::processEvents();
        QThread::msleep(100);
    }
    qDebug() << "MainWindowBrick shown, visibility:" << mainWindow.isVisible() << "geometry:" << mainWindow.geometry();

    int result = app.exec();

    qDebug() << "QApplication exec returned, shutting down";
    startupBrick->shutdown();
    delete startupBrick;
    sharedMemory.detach();
    logFile.close();
    qDebug() << "QApplication about to quit";
    return result;
}
