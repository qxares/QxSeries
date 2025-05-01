#include "mainwindowbrick.h"
#include <QDebug>
#include <QAction>
#include <QApplication>
#include <QScreen>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QPushButton>
#include <QMoveEvent>
#include <QResizeEvent>
#include <QProcess>
#include <QFile>
#include <QSettings>

MainWindowBrick::MainWindowBrick(QWidget *parent) 
    : QMainWindow(parent) {
    themeBrick = new ThemeBrick(qApp, this);
    interlinkBrick = new InterlinkBrick(this);
    isRaisingGroup = false;
    openAppCount = 0;

    setWindowTitle("QxCentre");
    setMinimumSize(600, 400);
    setAttribute(Qt::WA_QuitOnClose, false);

    QSettings settings("QxSeries", "QxCentre");
    QString theme = settings.value("theme", "dark").toString();
    themeBrick->toggleDarkTheme(theme == "dark");
    setupTaskbar();

    // Load window position
    QRect geometry = settings.value("window_position/QxCentre", QRect(0, 0, 800, 600)).toRect();
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int x = qMax(screenGeometry.left(), qMin(geometry.x(), screenGeometry.right() - geometry.width()));
    int y = qMax(screenGeometry.top(), qMin(geometry.y(), screenGeometry.bottom() - geometry.height()));
    int width = qMin(geometry.width(), screenGeometry.width());
    int height = qMin(geometry.height(), screenGeometry.height());
    setGeometry(x, y, width, height);
    if (x == 0 && y == 0) {
        centerWindow();
    }

    qDebug() << "QxCentre main window initialized, geometry:" << geometry;
    show();
    raise();
    QApplication::processEvents();
    qDebug() << "MainWindowBrick shown, visibility:" << isVisible() << "geometry:" << geometry;
}

MainWindowBrick::~MainWindowBrick() {
    if (interlinkBrick) {
        disconnect(interlinkBrick, &InterlinkBrick::windowStateChanged, this, &MainWindowBrick::updateTaskbarWindows);
        delete interlinkBrick;
        interlinkBrick = nullptr;
        qDebug() << "Deleted interlinkBrick in cleanup";
    }
    if (themeBrick) {
        delete themeBrick;
        themeBrick = nullptr;
        qDebug() << "Deleted themeBrick in cleanup";
    }
    if (taskbarDock) {
        delete taskbarDock;
        taskbarDock = nullptr;
        qDebug() << "Deleted taskbarDock in cleanup";
    }
    qDebug() << "MainWindowBrick destroyed";
}

void MainWindowBrick::setupTaskbar() {
    taskbarDock = new QDockWidget(this);
    taskbarDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    taskbarDock->setAllowedAreas(Qt::TopDockWidgetArea);
    taskbarDock->setTitleBarWidget(new QWidget());
    setCentralWidget(new QWidget());
    addDockWidget(Qt::TopDockWidgetArea, taskbarDock);

    taskbar = new QToolBar(taskbarDock);
    taskbar->setMovable(false);
    taskbar->setStyleSheet("QToolBar { spacing: 10px; padding: 5px; } QPushButton { min-width: 80px; min-height: 30px; }");
    taskbarDock->setWidget(taskbar);

    qxCentreMenu = new QMenu("QxCentre", taskbar);
    QMenu *preferencesMenu = qxCentreMenu->addMenu("Preferences");
    QMenu *themesMenu = preferencesMenu->addMenu("Themes");
    QAction *darkThemeAction = themesMenu->addAction("Dark");
    darkThemeAction->setCheckable(true);
    darkThemeAction->setChecked(themeBrick->isDarkTheme());
    QAction *lightThemeAction = themesMenu->addAction("Light");
    lightThemeAction->setCheckable(true);
    lightThemeAction->setChecked(!themeBrick->isDarkTheme());
    exitAction = qxCentreMenu->addAction("Exit");
    QPushButton *qxCentreButton = new QPushButton("QxCentre", taskbar);
    qxCentreButton->setMenu(qxCentreMenu);
    taskbar->addWidget(qxCentreButton);

    qxAppsMenu = new QMenu("QxApps", taskbar);
    QMenu *qxDocumentsMenu = qxAppsMenu->addMenu("QxDocuments");
    qxDocumentsMenu->addAction("QxWrite");
    qxDocumentsMenu->addAction("QxSheet");
    qxDocumentsMenu->addAction("QxNotes");
    QMenu *qxGraphicsMenu = qxAppsMenu->addMenu("QxGraphics");
    qxGraphicsMenu->addAction("QxDraw");
    qxGraphicsMenu->addAction("Images");
    qxGraphicsMenu->addAction("Photos");
    QMenu *qxAudioMenu = qxAppsMenu->addMenu("QxAudio");
    qxAudioMenu->addAction("QxAudioPlayer");
    qxAudioMenu->addAction("Music");
    qxAudioMenu->addAction("Books");
    qxAudioMenu->addAction("Recordings");
    QMenu *qxVideoMenu = qxAppsMenu->addMenu("QxVideo");
    qxVideoMenu->addAction("QxVideoPlayer");
    qxVideoMenu->addAction("Movies");
    qxVideoMenu->addAction("Series");
    qxVideoMenu->addAction("Recordings");
    QMenu *qxToolsMenu = qxAppsMenu->addMenu("QxTools");
    qxToolsMenu->addAction("QxCalc");
    qxToolsMenu->addAction("QxConvert");
    qxToolsMenu->addAction("QxTranslate");
    qxToolsMenu->addAction("QxCMD");
    QMenu *qxTechMenu = qxAppsMenu->addMenu("QxTech");
    qxTechMenu->addAction("QxNetworkMonitor");
    qxTechMenu->addAction("QxDisk Management");
    QPushButton *qxAppsButton = new QPushButton("QxApps", taskbar);
    qxAppsButton->setMenu(qxAppsMenu);
    taskbar->addWidget(qxAppsButton);

    helpMenu = new QMenu("Help", taskbar);
    helpMenu->addAction("Documentation");
    helpMenu->addAction("Check for Updates");
    QPushButton *helpButton = new QPushButton("Help", taskbar);
    helpButton->setMenu(helpMenu);
    taskbar->addWidget(helpButton);

    taskbar->addSeparator();
    windowList = new QComboBox(taskbar);
    windowList->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    taskbar->addWidget(windowList);

    connect(darkThemeAction, &QAction::toggled, this, [this, lightThemeAction](bool checked) {
        QSettings settings("QxSeries", "QxCentre");
        if (checked) {
            themeBrick->toggleDarkTheme(true);
            lightThemeAction->setChecked(false);
            settings.setValue("theme", "dark");
        } else {
            themeBrick->toggleDarkTheme(false);
            lightThemeAction->setChecked(true);
            settings.setValue("theme", "light");
        }
    });
    connect(lightThemeAction, &QAction::toggled, this, [this, darkThemeAction](bool checked) {
        QSettings settings("QxSeries", "QxCentre");
        if (checked) {
            themeBrick->toggleDarkTheme(false);
            darkThemeAction->setChecked(false);
            settings.setValue("theme", "light");
        } else {
            themeBrick->toggleDarkTheme(true);
            darkThemeAction->setChecked(true);
            settings.setValue("theme", "dark");
        }
    });
    connect(exitAction, &QAction::triggered, this, &MainWindowBrick::handleExit);
    connect(qxAppsMenu, &QMenu::triggered, this, &MainWindowBrick::launchApp);
    connect(helpMenu, &QMenu::triggered, this, &MainWindowBrick::launchApp);
    connect(windowList, QOverload<int>::of(&QComboBox::activated), this, &MainWindowBrick::activateWindow);
    connect(interlinkBrick, &InterlinkBrick::windowStateChanged, this, &MainWindowBrick::updateTaskbarWindows);
}

void MainWindowBrick::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int x = (screenGeometry.width() - width()) / 2 + screenGeometry.left();
    int y = (screenGeometry.height() - height()) / 2 + screenGeometry.top();
    move(x, y);
    qDebug() << "Centered QxCentre at:" << x << y;
}

void MainWindowBrick::moveEvent(QMoveEvent *event) {
    QMainWindow::moveEvent(event);
    QSettings settings("QxSeries", "QxCentre");
    settings.setValue("window_position/QxCentre", geometry());
}

void MainWindowBrick::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    QSettings settings("QxSeries", "QxCentre");
    settings.setValue("window_position/QxCentre", geometry());
}

void MainWindowBrick::raiseGroup() {
    if (isRaisingGroup) return;
    isRaisingGroup = true;
    qDebug() << "Starting raiseGroup, QxCentre visible:" << isVisible();

    auto appWindows = interlinkBrick->getAppWindows();
    QStringList staleKeys;
    for (auto it = appWindows.constBegin(); it != appWindows.constEnd(); ++it) {
        if (!it.value()) {
            staleKeys << it.key();
        }
    }
    for (const QString &key : staleKeys) {
        interlinkBrick->unregisterAppWindow(key);
        qDebug() << "Removed stale app window: " << key;
    }

    for (const QPointer<QWidget> &window : appWindows) {
        if (window && window->isVisible()) {
            window->raise();
            qDebug() << "Raised app window: " << appWindows.key(window);
        }
    }

    show();
    isRaisingGroup = false;
    qDebug() << "Grouped QxCentre behind all apps";
}

void MainWindowBrick::mousePressEvent(QMouseEvent *event) {
    QMainWindow::mousePressEvent(event);
    if (!isRaisingGroup) {
        raiseGroup();
    }
}

void MainWindowBrick::closeEvent(QCloseEvent *event) {
    qDebug() << "QxCentre closeEvent triggered, openAppCount:" << openAppCount;
    if (openAppCount > 0) {
        qDebug() << "Blocking QxCentre closure due to open apps";
        event->ignore();
        show();
        return;
    }
    event->accept();
    qDebug() << "QxCentre closeEvent accepted";
}

void MainWindowBrick::handleAppWindowDestroyed(QObject *obj) {
    qDebug() << "handleAppWindowDestroyed called for object:" << obj;
    auto appWindows = interlinkBrick->getAppWindows();
    for (auto it = appWindows.constBegin(); it != appWindows.constEnd(); ++it) {
        if (it.value() == obj) {
            themeBrick->disconnectThemeSignals(it.value());
            interlinkBrick->unregisterAppWindow(it.key());
            openAppCount--;
            qDebug() << "Handled destroyed app window: " << it.key() << ", openAppCount:" << openAppCount;
            break;
        }
    }
    if (isVisible()) {
        show();
        qDebug() << "Ensured QxCentre remains visible";
    }
}

void MainWindowBrick::handleExit() {
    qDebug() << "handleExit triggered";
    close();
    QApplication::quit();
}

void MainWindowBrick::launchApp(QAction *action) {
    QString appName = action->text();
    qDebug() << "Launch requested for: " << appName;
    interlinkBrick->launchAppWindow(appName);
}

void MainWindowBrick::activateWindow(int index) {
    if (index >= 0) {
        QString windowName = windowList->itemText(index);
        qDebug() << "Activating window: " << windowName;
        interlinkBrick->restoreWindow(windowName);
    }
}

void MainWindowBrick::updateTaskbarWindows() {
    if (windowList) {
        windowList->clear();
        auto appWindows = interlinkBrick->getAppWindows();
        for (auto it = appWindows.constBegin(); it != appWindows.constEnd(); ++it) {
            if (it.value() && it.value()->isVisible()) {
                windowList->addItem(it.key());
            }
        }
        qDebug() << "Updated taskbar window list";
    }
}

ThemeBrick* MainWindowBrick::getThemeBrick() {
    return themeBrick;
}

InterlinkBrick* MainWindowBrick::getInterlinkBrick() {
    return interlinkBrick;
}
