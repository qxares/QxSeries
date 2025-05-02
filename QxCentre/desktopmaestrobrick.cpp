#include "desktopmaestrobrick.h"
#include "mainwindowbrick.h"
#include <QDebug>
#include <QApplication>
#include <QTimer>

DesktopMaestroBrick* DesktopMaestroBrick::instance() {
    static DesktopMaestroBrick instance;
    return &instance;
}

DesktopMaestroBrick::DesktopMaestroBrick(QObject *parent) : QObject(parent) {}

void DesktopMaestroBrick::initializeDesktop() {
    qDebug() << "Initializing desktop";
    mainWindow = new MainWindowBrick();
    mainWindow->show();
    QApplication::processEvents();
    qDebug() << "Desktop shown, visible:" << mainWindow->isVisible();

    // Retry if not visible
    if (!mainWindow->isVisible()) {
        qDebug() << "Desktop not visible, retrying";
        delete mainWindow;
        mainWindow = new MainWindowBrick();
        mainWindow->show();
        QApplication::processEvents();
        qDebug() << "Desktop retry, visible:" << mainWindow->isVisible();
    }
}

bool DesktopMaestroBrick::isDesktopVisible() const {
    return mainWindow && mainWindow->isVisible();
}
