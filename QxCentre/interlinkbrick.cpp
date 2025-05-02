#include "interlinkbrick.h"
#include <QDebug>

InterlinkBrick::InterlinkBrick(QObject *parent)
    : QObject(parent), openAppCount(0) {
    qDebug() << "InterlinkBrick initialized";
}

InterlinkBrick::~InterlinkBrick() {
    qDebug() << "InterlinkBrick destroyed";
}

void InterlinkBrick::incrementOpenAppCount() {
    openAppCount++;
    qDebug() << "Incremented open app count:" << openAppCount;
    emit appCountChanged(openAppCount);
}

void InterlinkBrick::decrementOpenAppCount() {
    if (openAppCount > 0) {
        openAppCount--;
        qDebug() << "Decremented open app count:" << openAppCount;
        emit appCountChanged(openAppCount);
    }
}

void InterlinkBrick::activateWindow(int index) {
    if (index >= 0 && index < openWindows.size()) {
        WindowBrick *window = openWindows[index];
        if (window) {
            window->show();
            window->raise();
            window->activateWindow();
            qDebug() << "Activated window:" << window->windowTitle() << "at index:" << index;
        }
    }
}

void InterlinkBrick::updateTaskbarWindows() {
    QStringList windowTitles;
    for (const WindowBrick *window : openWindows) {
        if (window) {
            windowTitles << window->windowTitle();
        }
    }
    qDebug() << "Updating taskbar with windows:" << windowTitles;
    emit taskbarUpdated(windowTitles);
}

void InterlinkBrick::addWindow(WindowBrick *window) {
    if (window && !openWindows.contains(window)) {
        openWindows.append(window);
        connect(window, &WindowBrick::windowClosed, this, &InterlinkBrick::handleWindowClosed);
        incrementOpenAppCount();
        updateTaskbarWindows();
        qDebug() << "Added window:" << window->windowTitle();
    }
}

void InterlinkBrick::handleWindowClosed(WindowBrick *window) {
    if (window && openWindows.contains(window)) {
        openWindows.removeOne(window);
        disconnect(window, &WindowBrick::windowClosed, this, &InterlinkBrick::handleWindowClosed);
        decrementOpenAppCount();
        updateTaskbarWindows();
        qDebug() << "Handled window closed:" << window->windowTitle();
    }
}

int InterlinkBrick::getOpenAppCount() const {
    return openAppCount;
}
