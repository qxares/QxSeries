#include "windowmanagerbrick.h"
#include <QDebug>
#include <QEvent>
#include <QWindow>

WindowManagerBrick::WindowManagerBrick(QMainWindow *mainWindow, QObject *parent)
    : QObject(parent), mainWindow(mainWindow) {
    mainWindow->installEventFilter(this);
    qDebug() << "WindowManagerBrick initialized";
}

void WindowManagerBrick::registerAppWindow(QWidget *appWindow) {
    if (!appWindows.contains(appWindow)) {
        appWindows.append(appWindow);
        qDebug() << "Registered app window: " << appWindow->windowTitle();
    }
}

void WindowManagerBrick::launchAppWindow(QWidget *appWindow) {
    registerAppWindow(appWindow);
    appWindow->show();
    mainWindow->raise(); // QxCentre above other programs
    appWindow->raise(); // App above QxCentre
    appWindow->windowHandle()->requestActivate(); // Ensure focus
    qDebug() << "Launched app window: " << appWindow->windowTitle();
}

void WindowManagerBrick::handleMainWindowActivated() {
    qDebug() << "QxCentre window activated";
    raiseAllWindows();
}

void WindowManagerBrick::raiseAllWindows() {
    mainWindow->raise(); // QxCentre above other programs
    for (QWidget *appWindow : appWindows) {
        if (appWindow->isWindow() && !appWindow->isHidden()) {
            appWindow->raise(); // Apps above QxCentre
            appWindow->windowHandle()->requestActivate(); // Ensure focus
            qDebug() << "Raised app window: " << appWindow->windowTitle();
        }
    }
}

bool WindowManagerBrick::eventFilter(QObject *obj, QEvent *event) {
    if (obj == mainWindow && event->type() == QEvent::WindowActivate) {
        handleMainWindowActivated();
    }
    return QObject::eventFilter(obj, event);
}
