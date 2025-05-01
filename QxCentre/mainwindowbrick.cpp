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
#include <QMenu>
#include "infodialogbrick.h"
#include "interlinkbrick.h"

MainWindowBrick::MainWindowBrick(QWidget *parent) 
    : QMainWindow(parent) {
    qDebug() << "MainWindowBrick constructor started";
    themeBrick = new ThemeBrick(qApp, this);
    qDebug() << "ThemeBrick created";
    interlinkBrick = new InterlinkBrick(this);
    qDebug() << "InterlinkBrick created";
    isRaisingGroup = false;
    openAppCount = 0;

    setWindowTitle("qxcentre");
    setMinimumSize(600, 400);
    setAttribute(Qt::WA_QuitOnClose, false);

    QSettings settings("QxSeries", "qxcentre");
    QString theme = settings.value("theme", "dark").toString();
    themeBrick->toggleDarkTheme(theme == "dark");
    qDebug() << "Theme set to:" << theme;

    setupTaskbar();
    qDebug() << "Taskbar setup complete";

    // Maximize window dynamically
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    QRect geometry = settings.value("window_position/qxcentre").toRect();
    if (!geometry.isValid()) {
        int width = screenGeometry.width() * 0.9;
        int height = screenGeometry.height() * 0.9;
        geometry = QRect(0, 0, width, height);
    }
    int x = qMax(screenGeometry.left(), qMin(geometry.x(), screenGeometry.right() - geometry.width()));
    int y = qMax(screenGeometry.top(), qMin(geometry.y(), screenGeometry.bottom() - geometry.height()));
    int width = qMin(geometry.width(), static_cast<int>(screenGeometry.width() * 0.9));
    int height = qMin(geometry.height(), static_cast<int>(screenGeometry.height() * 0.9));
    setGeometry(x, y, width, height);
    if (x == 0 && y == 0) {
        centerWindow();
    }
    showMaximized(); // Maximize window
    qDebug() << "qxcentre main window initialized, geometry:" << geometry;

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
    // Remove centralWidget to avoid overlap
    // setCentralWidget(new QWidget());
    addDockWidget(Qt::TopDockWidgetArea, taskbarDock);

    taskbar = new QToolBar(taskbarDock);
    taskbar->setMovable(false);
    taskbar->setStyleSheet("QToolBar { spacing: 10px; padding: 5px; } QPushButton { min-width: 80px; min-height: 30px; }");
    taskbarDock->setWidget(taskbar);

    qxCentreMenu = new QMenu("qxcentre", taskbar);
    QMenu *preferencesMenu = qxCentreMenu->addMenu("Preferences");
    QMenu *themesMenu = preferencesMenu->addMenu("Themes");
    QAction *darkThemeAction = themesMenu->addAction("Dark");
    darkThemeAction->setCheckable(true);
    darkThemeAction->setChecked(themeBrick->isDarkTheme());
    QAction *lightThemeAction = themesMenu->addAction("Light");
    lightThemeAction->setCheckable(true);
    lightThemeAction->setChecked(!themeBrick->isDarkTheme());
    exitAction = qxCentreMenu->addAction("Exit");
    QPushButton *qxCentreButton = new QPushButton("qxcentre", taskbar);
    qxCentreButton->setMenu(qxCentreMenu);
    taskbar->addWidget(qxCentreButton);

    qxAppsMenu = new QMenu("QxApps", taskbar);
    QMenu *qxDocumentsMenu = qxAppsMenu->addMenu("QxDocuments");
    QMenu *qxWriteMenu = qxDocumentsMenu->addMenu("QxWrite");
    qxWriteMenu->addAction("Info");
    QMenu *qxSheetMenu = qxDocumentsMenu->addMenu("QxSheet");
    qxSheetMenu->addAction("Info");
    QMenu *qxNotesMenu = qxDocumentsMenu->addMenu("QxNotes");
    qxNotesMenu->addAction("Info");
    QMenu *qxGraphicsMenu = qxAppsMenu->addMenu("QxGraphics");
    QMenu *qxDrawMenu = qxGraphicsMenu->addMenu("QxDraw");
    qxDrawMenu->addAction("Info");
    QMenu *imagesMenu = qxGraphicsMenu->addMenu("Images");
    imagesMenu->addAction("Info");
    QMenu *photosMenu = qxGraphicsMenu->addMenu("Photos");
    photosMenu->addAction("Info");
    QMenu *qxAudioMenu = qxAppsMenu->addMenu("QxAudio");
    QMenu *qxAudioPlayerMenu = qxAudioMenu->addMenu("QxAudioPlayer");
    qxAudioPlayerMenu->addAction("Info");
    QMenu *musicMenu = qxAudioMenu->addMenu("Music");
    musicMenu->addAction("Info");
    QMenu *booksMenu = qxAudioMenu->addMenu("Books");
    booksMenu->addAction("Info");
    QMenu *recordingsAudioMenu = qxAudioMenu->addMenu("Recordings");
    recordingsAudioMenu->addAction("Info");
    QMenu *qxVideoMenu = qxAppsMenu->addMenu("QxVideo");
    QMenu *qxVideoPlayerMenu = qxVideoMenu->addMenu("QxVideoPlayer");
    qxVideoPlayerMenu->addAction("Info");
    QMenu *moviesMenu = qxVideoMenu->addMenu("Movies");
    moviesMenu->addAction("Info");
    QMenu *seriesMenu = qxVideoMenu->addMenu("Series");
    seriesMenu->addAction("Info");
    QMenu *recordingsVideoMenu = qxVideoMenu->addMenu("Recordings");
    recordingsVideoMenu->addAction("Info");
    QMenu *qxToolsMenu = qxAppsMenu->addMenu("QxTools");
    QMenu *qxCalcMenu = qxToolsMenu->addMenu("QxCalc");
    qxCalcMenu->addAction("Info");
    QMenu *qxConvertMenu = qxToolsMenu->addMenu("QxConvert");
    qxConvertMenu->addAction("Info");
    QMenu *qxTranslateMenu = qxToolsMenu->addMenu("QxTranslate");
    qxTranslateMenu->addAction("Info");
    QMenu *qxCMDMenu = qxToolsMenu->addMenu("QxCMD");
    qxCMDMenu->addAction("Info");
    QMenu *qxTechMenu = qxAppsMenu->addMenu("QxTech");
    QMenu *qxNetworkMonitorMenu = qxTechMenu->addMenu("QxNetworkMonitor");
    qxNetworkMonitorMenu->addAction("Info");
    QMenu *qxDiskManagementMenu = qxTechMenu->addMenu("QxDisk Management");
    qxDiskManagementMenu->addAction("Info");
    QPushButton *qxAppsButton = new QPushButton("QxApps", taskbar);
    qxAppsButton->setMenu(qxAppsMenu);
    taskbar->addWidget(qxAppsButton);

    helpMenu = new QMenu("Help", taskbar);
    helpMenu->addAction("Documentation");
    helpMenu->addAction("Check for Updates");
    helpMenu->addAction("Information");
    QPushButton *helpButton = new QPushButton("Help", taskbar);
    helpButton->setMenu(helpMenu);
    taskbar->addWidget(helpButton);

    taskbar->addSeparator();
    windowList = new QComboBox(taskbar);
    windowList->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    taskbar->addWidget(windowList);

    connect(darkThemeAction, &QAction::toggled, this, [this, lightThemeAction](bool checked) {
        QSettings settings("QxSeries", "qxcentre");
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
        QSettings settings("QxSeries", "qxcentre");
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
    connect(qxAppsMenu, &QMenu::triggered, this, [this](QAction *action) {
        if (action->text() == "Info") {
            QMenu *parentMenu = qobject_cast<QMenu*>(action->parentWidget());
            if (parentMenu) {
                QString appName = parentMenu->title();
                qDebug() << "Launching info window for:" << appName;
                interlinkBrick->launchAppWindow(appName + "_Info");
            } else {
                qDebug() << "Error: Failed to cast parentWidget to QMenu";
            }
        }
    });
    connect(helpMenu, &QMenu::triggered, this, [this](QAction *action) {
        if (action->text() == "Information") {
            launchInfoWindow();
        } else {
            interlinkBrick->launchAppWindow(action->text());
        }
    });
    connect(windowList, QOverload<int>::of(&QComboBox::activated), this, &MainWindowBrick::activateWindow);
    connect(interlinkBrick, &InterlinkBrick::windowStateChanged, this, &MainWindowBrick::updateTaskbarWindows);
}

void MainWindowBrick::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    QPoint cursorPos = QCursor::pos();
    for (QScreen *s : QGuiApplication::screens()) {
        if (s->geometry().contains(cursorPos)) {
            screen = s;
            screenGeometry = s->availableGeometry();
            break;
        }
    }
    int x = (screenGeometry.width() - width()) / 2 + screenGeometry.left();
    int y = (screenGeometry.height() - height()) / 2 + screenGeometry.top();
    move(x, y);
    QSettings settings("QxSeries", "qxcentre");
    settings.setValue("window_position/qxcentre", geometry());
    qDebug() << "Centered qxcentre at:" << x << y << "on screen:" << screen->name();
}

void MainWindowBrick::moveEvent(QMoveEvent *event) {
    QMainWindow::moveEvent(event);
    QSettings settings("QxSeries", "qxcentre");
    settings.setValue("window_position/qxcentre", geometry());
}

void MainWindowBrick::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    QSettings settings("QxSeries", "qxcentre");
    settings.setValue("window_position/qxcentre", geometry());
}

void MainWindowBrick::raiseGroup() {
    if (isRaisingGroup) return;
    isRaisingGroup = true;
    qDebug() << "Starting raiseGroup, qxcentre visible:" << isVisible();

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
    qDebug() << "Grouped qxcentre behind all apps";
}

void MainWindowBrick::mousePressEvent(QMouseEvent *event) {
    QMainWindow::mousePressEvent(event);
    if (!isRaisingGroup) {
        raiseGroup();
    }
}

void MainWindowBrick::closeEvent(QCloseEvent *event) {
    qDebug() << "qxcentre closeEvent triggered, openAppCount:" << openAppCount;
    if (openAppCount > 0) {
        qDebug() << "Blocking qxcentre closure due to open apps";
        event->ignore();
        show();
        return;
    }
    event->accept();
    qDebug() << "qxcentre closeEvent accepted";
}

void MainWindowBrick::handleAppWindowDestroyed(QObject *obj) {
    qDebug() << "handleAppWindowDestroyed called for object:" << obj;
    auto appWindows = interlinkBrick->getAppWindows();
    for (auto it = appWindows.constBegin(); it != appWindows.constEnd(); ++it) {
        if (it.value() == obj) {
            QString key = it.key();
            if (themeBrick && it.value()) {
                disconnect(themeBrick, &ThemeBrick::themeChanged, it.value(), nullptr);
                qDebug() << "Disconnected theme signals for" << key;
            }
            interlinkBrick->unregisterAppWindow(key);
            decrementOpenAppCount();
            qDebug() << "Handled destroyed app window:" << key << ", openAppCount:" << openAppCount;
            break;
        }
    }
    if (isVisible()) {
        show();
        qDebug() << "Ensured qxcentre remains visible";
    }
}

void MainWindowBrick::incrementOpenAppCount() {
    openAppCount++;
    qDebug() << "Incremented openAppCount to:" << openAppCount;
}

void MainWindowBrick::decrementOpenAppCount() {
    if (openAppCount > 0) {
        openAppCount--;
        qDebug() << "Decremented openAppCount to:" << openAppCount;
    }
}

void MainWindowBrick::handleExit() {
    qDebug() << "handleExit triggered";
    close();
    QApplication::quit();
}

void MainWindowBrick::launchInfoWindow() {
    qDebug() << "Launching info window";
    interlinkBrick->launchAppWindow("Information");
}

void MainWindowBrick::activateWindow(int index) {
    if (index >= 0) {
        QString windowName = windowList->itemText(index);
        qDebug() << "Activating window:" << windowName;
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
