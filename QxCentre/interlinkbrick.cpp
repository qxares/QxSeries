#include "interlinkbrick.h"
#include "mainwindowbrick.h"
#include <QDebug>

InterlinkBrick::InterlinkBrick(MainWindowBrick *mainWindow, QObject *parent)
    : QObject(parent), mainWindow(mainWindow) {
    qDebug() << "InterlinkBrick initialized";
}

InterlinkBrick::~InterlinkBrick() {
    clearAppWindows();
    qDebug() << "InterlinkBrick destroyed";
}

void InterlinkBrick::registerAppWindow(const QString &name, QWidget *window) {
    appWindows.insert(name, window);
    qDebug() << "Registered app window: " << name;
}

void InterlinkBrick::unregisterAppWindow(const QString &name) {
    if (appWindows.contains(name)) {
        if (appWindows[name]) {
            disconnect(appWindows[name], nullptr, nullptr, nullptr);
        }
        appWindows.remove(name);
        qDebug() << "Unregistered app window: " << name;
    }
}

void InterlinkBrick::clearAppWindows() {
    for (auto it = appWindows.constBegin(); it != appWindows.constEnd(); ++it) {
        if (it.value()) {
            disconnect(it.value(), nullptr, nullptr, nullptr);
        }
    }
    appWindows.clear();
    qDebug() << "Cleared all app windows";
}

void InterlinkBrick::launchAppWindow(const QString &name) {
    if (appWindows.contains(name) && appWindows[name]) {
        appWindows[name]->show();
        qDebug() << "Launched app window: " << name;
    }
}

QMap<QString, QPointer<QWidget>> InterlinkBrick::getAppWindows() const {
    return appWindows;
}

void InterlinkBrick::raiseGroup() {
    if (mainWindow) {
        mainWindow->raiseGroup();
    }
}
