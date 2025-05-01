#include "interlinkbrick.h"
#include <QDebug>
#include <QApplication>
#include <QScreen>
#include <QSettings>
#include "infodialogbrick.h"
#include "mainwindowbrick.h"

InterlinkBrick::InterlinkBrick(MainWindowBrick *parent)
    : QObject(parent), mainWindow(parent) {
    appWindows.clear();
    qDebug() << "InterlinkBrick initialized";
}

InterlinkBrick::~InterlinkBrick() {
    qDebug() << "InterlinkBrick destroyed";
}

void InterlinkBrick::registerAppWindow(const QString &appName, QWidget *window) {
    if (!appWindows.contains(appName) && window) {
        appWindows.insert(appName, window);
        ThemeBrick *themeBrick = mainWindow->getThemeBrick();
        if (themeBrick) {
            connect(themeBrick, &ThemeBrick::themeChanged, window, [=]() {
                themeBrick->toggleDarkTheme(themeBrick->isDarkTheme());
            });
        }
        QSettings settings("QxSeries", "qxcentre");
        QString key = QString("window_position/%1").arg(appName);
        QRect geometry = settings.value(key).toRect();
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->availableGeometry();
        if (geometry.isValid()) {
            int x = qMax(screenGeometry.left(), qMin(geometry.x(), screenGeometry.right() - geometry.width()));
            int y = qMax(screenGeometry.top(), qMin(geometry.y(), screenGeometry.bottom() - geometry.height()));
            int width = qMin(geometry.width(), screenGeometry.width());
            int height = qMin(geometry.height(), screenGeometry.height());
            window->setGeometry(x, y, width, height);
        } else {
            // Center 300x200 window
            int x = (screenGeometry.width() - 300) / 2 + screenGeometry.left();
            int y = (screenGeometry.height() - 200) / 2 + screenGeometry.top();
            window->setGeometry(x, y, 300, 200);
        }
        connect(window, &QWidget::destroyed, this, [=]() {
            unregisterAppWindow(appName);
        });
        qDebug() << "Registered app" << appName << "with dependencies: theme,geometry";
        emit windowStateChanged();
    }
}

void InterlinkBrick::unregisterAppWindow(const QString &appName) {
    if (appWindows.contains(appName)) {
        appWindows.remove(appName);
        qDebug() << "Unregistered app window:" << appName;
        emit windowStateChanged();
    }
}

void InterlinkBrick::restoreWindow(const QString &appName) {
    if (appWindows.contains(appName) && appWindows[appName]) {
        QWidget *window = appWindows[appName];
        if (window->isMinimized()) {
            window->showNormal();
        }
        window->raise();
        window->activateWindow();
        qDebug() << "Restored window:" << appName;
        emit windowStateChanged();
    }
}

void InterlinkBrick::launchAppWindow(const QString &appName) {
    qDebug() << "Launching app window:" << appName;
    InfoDialogBrick *window = nullptr;
    if (appName == "Information") {
        window = new InfoDialogBrick("Information", "Created by:\nP. van den Bosch and Grok xAI", nullptr);
    } else {
        window = new InfoDialogBrick(appName, "App is still in the garage\n🚗💨", nullptr);
    }
    window->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(window, &QObject::destroyed, mainWindow, &MainWindowBrick::handleAppWindowDestroyed);
    registerAppWindow(appName, window);
    mainWindow->incrementOpenAppCount();
    window->show();
    window->raise();
    qDebug() << "Launched" << appName << "with geometry:" << window->geometry();
}

QMap<QString, QPointer<QWidget>> InterlinkBrick::getAppWindows() const {
    return appWindows;
}
