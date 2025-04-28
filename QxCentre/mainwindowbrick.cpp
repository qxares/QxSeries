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

MainWindowBrick::MainWindowBrick(QWidget *parent) : QMainWindow(parent) {
    themeBrick = new ThemeBrick(qApp, this);
    interlinkBrick = new InterlinkBrick(this);
    databaseBrick = new DatabaseBrick(this);
    isRaisingGroup = false;
    openAppCount = 0;
    
    if (!databaseBrick->initialize()) {
        qDebug() << "Failed to initialize DatabaseBrick";
    }

    setWindowFlags(Qt::Window | Qt::WindowStaysOnBottomHint);
    setWindowTitle("QxCentre");
    setMinimumSize(600, 400);
    setAttribute(Qt::WA_QuitOnClose, false);

    // Load theme
    QString theme = databaseBrick->loadTheme();
    themeBrick->toggleDarkTheme(theme == "dark");
    setupTaskbar();

    // Load window position
    int x, y, width, height;
    if (databaseBrick->loadWindowPosition("QxCentre", x, y, width, height)) {
        setGeometry(x, y, width, height);
    } else {
        resize(1050, 800);
        moveToBottomLeft();
    }

    qDebug() << "QxCentre main window initialized";
}

MainWindowBrick::~MainWindowBrick() {
    if (taskbarDock) {
        delete taskbarDock;
        taskbarDock = nullptr;
        qDebug() << "Deleted taskbarDock in destructor";
    }
    if (themeBrick) {
        delete themeBrick;
        themeBrick = nullptr;
        qDebug() << "Deleted themeBrick in destructor";
    }
    if (interlinkBrick) {
        delete interlinkBrick;
        interlinkBrick = nullptr;
        qDebug() << "Deleted interlinkBrick in destructor";
    }
    if (databaseBrick) {
        delete databaseBrick;
        databaseBrick = nullptr;
        qDebug() << "Deleted databaseBrick in destructor";
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

    // QxCentre Menu
    qxCentreMenu = new QMenu("QxCentre", taskbar);
    QMenu *preferencesMenu = qxCentreMenu->addMenu("Preferences");
    QMenu *themesMenu = preferencesMenu->addMenu("Themes");
    QAction *darkThemeAction = themesMenu->addAction("Dark");
    darkThemeAction->setCheckable(true);
    darkThemeAction->setChecked(themeBrick->isDarkTheme());
    QAction *lightThemeAction = themesMenu->addAction("Light");
    lightThemeAction->setCheckable(true);
    lightThemeAction->setChecked(!themeBrick->isDarkTheme());
    QMenu *databaseMenu = qxCentreMenu->addMenu("DataBase");
    databaseMenu->addAction("New Database");
    databaseMenu->addAction("Open Database");
    databaseMenu->addAction("Save Database");
    databaseMenu->addAction("Save As");
    qxCentreMenu->addSeparator();
    exitAction = qxCentreMenu->addAction("Exit");
    QPushButton *qxCentreButton = new QPushButton("QxCentre", taskbar);
    qxCentreButton->setMenu(qxCentreMenu);
    taskbar->addWidget(qxCentreButton);

    // QxApps Menu
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

    // Help Menu
    helpMenu = new QMenu("Help", taskbar);
    helpMenu->addAction("About QxCentre");
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
        if (checked) {
            themeBrick->toggleDarkTheme(true);
            lightThemeAction->setChecked(false);
            databaseBrick->saveTheme("dark");
        } else {
            themeBrick->toggleDarkTheme(false);
            lightThemeAction->setChecked(true);
            databaseBrick->saveTheme("light");
        }
    });
    connect(lightThemeAction, &QAction::toggled, this, [this, darkThemeAction](bool checked) {
        if (checked) {
            themeBrick->toggleDarkTheme(false);
            darkThemeAction->setChecked(false);
            databaseBrick->saveTheme("light");
        } else {
            themeBrick->toggleDarkTheme(true);
            darkThemeAction->setChecked(true);
            databaseBrick->saveTheme("dark");
        }
    });
    connect(exitAction, &QAction::triggered, this, &MainWindowBrick::handleExit);
    connect(qxAppsMenu, &QMenu::triggered, this, &MainWindowBrick::launchApp);
    connect(databaseMenu, &QMenu::triggered, this, &MainWindowBrick::launchApp);
    connect(helpMenu, &QMenu::triggered, this, [this](QAction *action) {
        if (action->text() == "About QxCentre") {
            showAboutDialog();
        } else {
            launchApp(action);
        }
    });
    connect(windowList, QOverload<int>::of(&QComboBox::activated), this, &MainWindowBrick::activateWindow);
    connect(interlinkBrick, &InterlinkBrick::windowStateChanged, this, &MainWindowBrick::updateTaskbarWindows);
}

void MainWindowBrick::moveToBottomLeft() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = 0;
    int y = screenGeometry.height() - height();
    move(x, y);
}

void MainWindowBrick::moveEvent(QMoveEvent *event) {
    QMainWindow::moveEvent(event);
    databaseBrick->saveWindowPosition("QxCentre", x(), y(), width(), height());
}

void MainWindowBrick::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    databaseBrick->saveWindowPosition("QxCentre", x(), y(), width(), height());
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
    QMessageBox::information(this, "App Status", "App is in development, wait for it 😺");
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
    windowList->clear();
    auto appWindows = interlinkBrick->getAppWindows();
    for (auto it = appWindows.constBegin(); it != appWindows.constEnd(); ++it) {
        if (it.value() && it.value()->isVisible()) {
            windowList->addItem(it.key());
        }
    }
    qDebug() << "Updated taskbar window list";
}

void MainWindowBrick::showAboutDialog() {
    QMessageBox::about(this, "About QxCentre", "Created by Peter van den Bosch and Grok xAI");
}

ThemeBrick* MainWindowBrick::getThemeBrick() {
    return themeBrick;
}

InterlinkBrick* MainWindowBrick::getInterlinkBrick() {
    return interlinkBrick;
}

DatabaseBrick* MainWindowBrick::getDatabaseBrick() {
    return databaseBrick;
}
