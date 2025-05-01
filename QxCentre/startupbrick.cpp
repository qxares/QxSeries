#include "startupbrick.h"
#include <QDebug>
#include <QSettings>
#include <QRect>
#include <QElapsedTimer>

StartupBrick::StartupBrick(QObject *parent) 
    : QObject(parent) {
    qDebug() << "StartupBrick initialized";
}

StartupBrick::~StartupBrick() {
    shutdown();
    qDebug() << "StartupBrick destroyed";
}

bool StartupBrick::initialize() {
    QElapsedTimer timer;
    timer.start();

    if (!initSettings()) {
        qDebug() << "StartupBrick failed to initialize settings after" << timer.elapsed() << "ms";
        return false;
    }
    qDebug() << "Settings initialized in" << timer.elapsed() << "ms";

    qDebug() << "StartupBrick initialized successfully in" << timer.elapsed() << "ms";
    return true;
}

bool StartupBrick::initSettings() {
    QSettings settings("QxSeries", "QxCentre");
    if (!settings.contains("theme")) {
        settings.setValue("theme", "dark");
        qDebug() << "Initialized default theme: dark";
    }
    if (!settings.contains("window_position/QxCentre")) {
        settings.setValue("window_position/QxCentre", QRect(0, 0, 800, 600));
        qDebug() << "Initialized default window position for QxCentre";
    }
    qDebug() << "Settings initialized: theme=" << settings.value("theme").toString();
    return true;
}

void StartupBrick::shutdown() {
    qDebug() << "StartupBrick shutdown";
}
