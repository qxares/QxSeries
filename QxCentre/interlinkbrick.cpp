#include "interlinkbrick.h"
#include <QDebug>
#include <QEvent>
#include <QWindow>

InterlinkBrick::InterlinkBrick(QMainWindow *mainWindow, QObject *parent)
    : QObject(parent), mainWindow(mainWindow) {
    mainWindow->installEventFilter(this);
    qDebug() << "InterlinkBrick initialized";
}

void InterlinkBrick::registerAppWindow(QWidget *appWindow) {
    if (appWindow && !appWindows.contains(appWindow)) {
        appWindows.append(appWindow);
        connect(appWindow, &QObject::destroyed, this, &InterlinkBrick::onAppWindowDestroyed);
        qDebug() << "Registered app window: " << appWindow->windowTitle();
    }
}

void InterlinkBrick::launchAppWindow(QWidget *appWindow) {
    if (!appWindow) return;
    registerAppWindow(appWindow);
    appWindow->show();
    mainWindow->raise(); // QxCentre above other programs
    appWindow->raise(); // App above QxCentre
    if (appWindow->windowHandle()) {
        appWindow->windowHandle()->requestActivate(); // Ensure focus
    }
    qDebug() << "Launched app window: " << appWindow->windowTitle();
}

void InterlinkBrick::handleMainWindowActivated() {
    qDebug() << "QxCentre window activated";
    raiseAllWindows();
}

void InterlinkBrick::onAppWindowDestroyed(QObject *obj) {
    QWidget *appWindow = static_cast<QWidget*>(obj);
    appWindows.removeAll(appWindow);
    appWindowStates.remove(appWindow);
    qDebug() << "Unregistered app window: " << appWindow->windowTitle();
}

void InterlinkBrick::raiseAllWindows() {
    if (!mainWindow) return;
    mainWindow->raise(); // QxCentre above other programs
    for (QWidget *appWindow : appWindows) {
        if (appWindow && appWindow->isWindow() && !appWindow->isHidden() && appWindow->windowHandle()) {
            appWindow->raise(); // Apps above QxCentre
            appWindow->windowHandle()->requestActivate(); // Ensure focus
            qDebug() << "Raised app window: " << appWindow->windowTitle();
        }
    }
}

void InterlinkBrick::minimizeAllWindows() {
    for (QWidget *appWindow : appWindows) {
        if (appWindow && appWindow->isWindow() && !appWindow->isHidden() && appWindow->windowHandle()) {
            // Save current state before minimizing
            appWindowStates[appWindow] = appWindow->windowState();
            appWindow->showMinimized();
            qDebug() << "Minimized app window: " << appWindow->windowTitle();
        }
    }
}

void InterlinkBrick::restoreAllWindows() {
    for (QWidget *appWindow : appWindows) {
        if (appWindow && appWindow->isWindow() && appWindow->windowHandle()) {
            // Restore to previous state (normal or maximized)
            Qt::WindowStates prevState = appWindowStates.value(appWindow, Qt::WindowNoState);
            if (prevState & Qt::WindowMaximized) {
                appWindow->showMaximized();
            } else {
                appWindow->showNormal();
            }
            appWindow->raise(); // Ensure apps stay above QxCentre
            qDebug() << "Restored app window: " << appWindow->windowTitle();
        }
    }
}

bool InterlinkBrick::eventFilter(QObject *obj, QEvent *event) {
    if (obj == mainWindow) {
        if (event->type() == QEvent::WindowActivate) {
            handleMainWindowActivated();
        } else if (event->type() == QEvent::WindowStateChange) {
            bool isMinimized = mainWindow->windowState() & Qt::WindowMinimized;
            emit windowStateChanged(isMinimized);
            if (isMinimized) {
                minimizeAllWindows();
                qDebug() << "QxCentre minimized, syncing child apps";
            } else {
                restoreAllWindows();
                qDebug() << "QxCentre restored, syncing child apps";
            }
        }
    }
    return QObject::eventFilter(obj, event);
}
