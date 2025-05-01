#include <QApplication>
#include <QSharedMemory>
#include <QUuid>
#include <QDebug>
#include "mainwindowbrick.h"
#include "startupbrick.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qDebug() << "QApplication initialized";

    QString sharedMemoryKey = "QxCentreSharedMemory_" + QUuid::createUuid().toString();
    QSharedMemory sharedMemory(sharedMemoryKey);
    if (!sharedMemory.create(1024)) {
        qDebug() << "Failed to create QSharedMemory:" << sharedMemory.errorString();
        return 1;
    }
    qDebug() << "Using unique QSharedMemory key:" << sharedMemoryKey;

    // Clean up any stale shared memory
    QStringList staleKeys;
    QSharedMemory temp("qxcentre");
    if (temp.attach()) {
        temp.detach();
        staleKeys << "qxcentre";
    }
    qDebug() << "Cleared system-level QSharedMemory locks:" << staleKeys.join(",");

    StartupBrick startup;
    qDebug() << "StartupBrick initialized";

    MainWindowBrick *mainWindow = new MainWindowBrick();
    qDebug() << "MainWindowBrick created";

    int result = app.exec();
    qDebug() << "QApplication exec returned, shutting down";

    delete mainWindow;
    sharedMemory.detach();
    return result;
}
