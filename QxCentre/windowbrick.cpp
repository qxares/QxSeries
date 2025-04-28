#include "windowbrick.h"
#include "mainwindowbrick.h"
#include <QDebug>
#include <QCloseEvent>

WindowBrick::WindowBrick(MainWindowBrick *mainWindow, QWidget *parent)
    : QMainWindow(parent), mainWindowBrick(mainWindow) {
    if (mainWindowBrick) {
        themeBrick = mainWindowBrick->getThemeBrick();
        interlinkBrick = mainWindowBrick->getInterlinkBrick();
        if (themeBrick) {
            connect(themeBrick, &ThemeBrick::themeChanged, this, &WindowBrick::initializeTheme);
        }
    }
    setMinimumSize(400, 300);
    qDebug() << "WindowBrick initialized";
}

WindowBrick::~WindowBrick() {
    if (themeBrick) {
        themeBrick->disconnectThemeSignals(this);
    }
    qDebug() << "WindowBrick destroyed";
}

void WindowBrick::postInitialize() {
    if (themeBrick) {
        initializeTheme(themeBrick->isDarkTheme());
    }
}

void WindowBrick::closeEvent(QCloseEvent *event) {
    qDebug() << "WindowBrick closeEvent triggered for" << windowTitle();
    if (interlinkBrick) {
        interlinkBrick->unregisterAppWindow(windowTitle());
    }
    if (themeBrick) {
        themeBrick->disconnectThemeSignals(this);
    }
    event->accept();
}
