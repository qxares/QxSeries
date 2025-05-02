#include "mainwindowbrick.h"
#include "infodialogbrick.h"
#include <QDebug>
#include <QApplication>
#include <QScreen>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QWidget>
#include <QSettings>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QProcess>
#include <QFile>
#include <QDateTime>
#include "themebrick.h"
#include "windowbrick.h"

MainWindowBrick::MainWindowBrick(QWidget *parent) 
    : QMainWindow(parent), themeBrick(new ThemeBrick(qApp, this)), interlinkBrick(new InterlinkBrick(this)) {
    qDebug() << "MainWindowBrick constructor started";
    
    // Initialize logging
    QFile logFile("/home/ares/QxCentre/QxCentre/QxCentre.log");
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qInstallMessageHandler([](QtMsgType, const QMessageLogContext &, const QString &msg) {
            QFile logFile("/home/ares/QxCentre/QxCentre/QxCentre.log");
            if (logFile.open(QIODevice::Append | QIODevice::Text)) {
                QTextStream out(&logFile);
                out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " [DEBUG] " << msg << "\n";
            }
        });
        qDebug() << "Logging initialized to QxCentre.log";
    }

    setWindowTitle("QxCentre");
    setMinimumSize(600, 400);
    setAttribute(Qt::WA_QuitOnClose, true);
    setAttribute(Qt::WA_NativeWindow, true); // Ensure Muffin decorations

    // Log platform, display, and window manager
    qDebug() << "Platform:" << QGuiApplication::platformName() << "Display:" << qgetenv("DISPLAY");
    qDebug() << "Window manager:" << qgetenv("XDG_CURRENT_DESKTOP");

    // Initialize geometry dynamically
    QSettings settings("QxSeries", "QxCentre");
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen ? screen->availableGeometry() : QRect(0, 0, 1920, 1080);
    qDebug() << "Primary screen:" << (screen ? screen->name() : "none") << "Available geometry:" << screenGeometry;

    QRect geometry = settings.value("window_position/QxCentre", QRect(0, 0, screenGeometry.width(), screenGeometry.height())).toRect();
    qDebug() << "Saved geometry:" << geometry;
    if (!geometry.isValid() || geometry.width() > screenGeometry.width() || geometry.height() > screenGeometry.height()) {
        geometry = QRect(0, 0, screenGeometry.width(), screenGeometry.height());
        settings.setValue("window_position/QxCentre", geometry);
        qDebug() << "Reset geometry to:" << geometry;
    }
    setGeometry(geometry);
    qDebug() << "Set geometry:" << geometry;

    // Force maximize
    showMaximized();
    qDebug() << "Called showMaximized, maximized:" << isMaximized() << "geometry:" << geometry;
    if (!isMaximized()) {
        qDebug() << "showMaximized failed, forcing full screen geometry";
        setGeometry(screenGeometry);
    }

    // Add central widget
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    updateCentralWidgetStyle(themeBrick->isDarkTheme());
    connect(themeBrick, &ThemeBrick::themeChanged, this, &MainWindowBrick::updateCentralWidgetStyle);
    central->show();
    qDebug() << "Central widget set, visible:" << central->isVisible();

    // Setup taskbar
    setupTaskbar();

    // Show window
    show();
    raise();
    QMainWindow::activateWindow();
    QApplication::processEvents();
    qDebug() << "Window shown, visibility:" << isVisible() << "active:" << isActiveWindow() << "geometry:" << geometry;

    qDebug() << "MainWindowBrick constructor completed";
}

MainWindowBrick::~MainWindowBrick() {
    qDebug() << "MainWindowBrick destroyed";
}

void MainWindowBrick::setupTaskbar() {
    qDebug() << "Setting up taskbar";
    QMenuBar *taskbar = menuBar();
    updateCentralWidgetStyle(themeBrick->isDarkTheme()); // Apply initial theme to menu bar

    // QxCentre Menu
    QMenu *qxCentreMenu = taskbar->addMenu("QxCentre");
    QMenu *preferencesMenu = qxCentreMenu->addMenu("Preferences");
    QMenu *themesMenu = preferencesMenu->addMenu("Themes");
    QAction *darkAction = themesMenu->addAction("Dark");
    QAction *lightAction = themesMenu->addAction("Light");
    darkAction->setCheckable(true);
    lightAction->setCheckable(true);
    darkAction->setChecked(themeBrick->isDarkTheme());
    lightAction->setChecked(!themeBrick->isDarkTheme());
    QActionGroup *themeGroup = new QActionGroup(this);
    themeGroup->addAction(darkAction);
    themeGroup->addAction(lightAction);
    themeGroup->setExclusive(true);
    connect(darkAction, &QAction::triggered, this, [this]() {
        themeBrick->setTheme("dark");
        qDebug() << "Switched to dark theme";
    });
    connect(lightAction, &QAction::triggered, this, [this]() {
        themeBrick->setTheme("light");
        qDebug() << "Switched to light theme";
    });
    QMenu *databaseMenu = qxCentreMenu->addMenu("DataBase");
    databaseMenu->addAction("New Database");
    databaseMenu->addAction("Open Database");
    databaseMenu->addAction("Save Database");
    databaseMenu->addAction("Save As");
    QAction *exitAction = qxCentreMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &MainWindowBrick::handleExit);

    // QxApps Menu
    QMenu *qxAppsMenu = taskbar->addMenu("QxApps");
    QMenu *documentsMenu = qxAppsMenu->addMenu("QxDocuments");
    QAction *qxWriteAction = documentsMenu->addAction("QxWrite");
    // Disabled until QxWrite is implemented
    // connect(qxWriteAction, &QAction::triggered, this, [this]() {
    //     WindowBrick *window = new WindowBrick("QxWrite", this);
    //     window->initializeTheme(themeBrick->isDarkTheme());
    //     window->centerWindow();
    //     window->show();
    //     interlinkBrick->addWindow(window);
    //     qDebug() << "Launched QxWrite window";
    // });
    documentsMenu->addAction("QxSheet");
    documentsMenu->addAction("QxNotes");
    QMenu *graphicsMenu = qxAppsMenu->addMenu("QxGraphics");
    graphicsMenu->addAction("QxDraw");
    graphicsMenu->addAction("Images");
    graphicsMenu->addAction("Photos");
    QMenu *audioMenu = qxAppsMenu->addMenu("QxAudio");
    audioMenu->addAction("QxAudioPlayer");
    audioMenu->addAction("Music");
    audioMenu->addAction("Books");
    audioMenu->addAction("Recordings");
    QMenu *videoMenu = qxAppsMenu->addMenu("QxVideo");
    videoMenu->addAction("QxVideoPlayer");
    videoMenu->addAction("Movies");
    videoMenu->addAction("Series");
    audioMenu->addAction("Recordings");
    QMenu *toolsMenu = qxAppsMenu->addMenu("QxTools");
    toolsMenu->addAction("QxCalc");
    toolsMenu->addAction("QxConvert");
    toolsMenu->addAction("QxTranslate");
    toolsMenu->addAction("QxCMD");
    QMenu *techMenu = qxAppsMenu->addMenu("QxTech");
    techMenu->addAction("QxNetworkMonitor");
    techMenu->addAction("QxDisk Management");

    // Help Menu
    QMenu *helpMenu = taskbar->addMenu("Help");
    QAction *aboutAction = helpMenu->addAction("About QxCentre");
    QAction *docsAction = helpMenu->addAction("Documentation");
    QAction *updatesAction = helpMenu->addAction("Check for Updates");
    connect(aboutAction, &QAction::triggered, this, [this]() {
        InfoDialogBrick *dialog = new InfoDialogBrick("About QxCentre", "QxCentre v1.0\nA Qt-powered desktop environment for Aries-Desk.", this);
        dialog->show();
        qDebug() << "Showing About QxCentre dialog";
    });
    connect(docsAction, &QAction::triggered, this, [this]() {
        InfoDialogBrick *dialog = new InfoDialogBrick("Documentation", "QxCentre Documentation\nSee https://github.com/qxares/QxSeries for details.", this);
        dialog->show();
        qDebug() << "Showing Documentation dialog";
    });
    connect(updatesAction, &QAction::triggered, this, [this]() {
        InfoDialogBrick *dialog = new InfoDialogBrick("Check for Updates", "No updates available.\nCheck https://github.com/qxares/QxSeries for the latest version.", this);
        dialog->show();
        qDebug() << "Showing Check for Updates dialog";
    });

    // Connect InterlinkBrick signals
    connect(interlinkBrick, &InterlinkBrick::taskbarUpdated, this, &MainWindowBrick::updateTaskbarWindows);
    connect(interlinkBrick, &InterlinkBrick::appCountChanged, this, [this](int count) {
        qDebug() << "Open app count changed:" << count;
    });

    taskbar->show();
    qDebug() << "Taskbar set, visible:" << taskbar->isVisible();
}

void MainWindowBrick::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen ? screen->availableGeometry() : QRect(0, 0, 1920, 1080);
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
    QSettings settings("QxSeries", "QxCentre");
    settings.setValue("window_position/QxCentre", geometry());
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

void MainWindowBrick::updateCentralWidgetStyle(bool isDark) {
    QString bgColor = isDark ? "#272822" : "#FFFFFF"; // Dark gray-green or white
    QString textColor = isDark ? "#F8F8F2" : "#000000"; // Off-white or black
    QString menuBarBg = isDark ? "#1e1f1a" : "#F0F0F0"; // Darker gray-green or light gray
    QString hoverColor = isDark ? "#63634e" : "#0078D7"; // Olive-green or Windows blue
    QString styleSheet = QString(
        "QWidget { background-color: %1; color: %2; }"
        "QMenuBar { background-color: %3; color: %2; font-size: 14px; padding: 5px; }"
        "QMenuBar::item { padding: 5px 10px; }"
        "QMenuBar::item:selected { background-color: %4; }"
        "QMenu { background-color: %1; color: %2; font-size: 12px; }"
        "QMenu::item { padding: 5px 20px; }"
        "QMenu::item:selected { background-color: %4; }"
    ).arg(bgColor, textColor, menuBarBg, hoverColor);
    
    if (centralWidget()) {
        centralWidget()->setStyleSheet(styleSheet);
    }
    menuBar()->setStyleSheet(styleSheet);
    qDebug() << "Updated central widget style, dark:" << isDark << "bg:" << bgColor << "menuBarBg:" << menuBarBg;
}

void MainWindowBrick::raiseGroup() {}
void MainWindowBrick::mousePressEvent(QMouseEvent *event) { QMainWindow::mousePressEvent(event); }
void MainWindowBrick::closeEvent(QCloseEvent *event) {
    qDebug() << "QxCentre closeEvent triggered";
    event->accept();
    handleExit();
}
void MainWindowBrick::handleAppWindowDestroyed([[maybe_unused]] QObject *obj) {}
void MainWindowBrick::incrementOpenAppCount() {
    interlinkBrick->incrementOpenAppCount();
}
void MainWindowBrick::decrementOpenAppCount() {
    interlinkBrick->decrementOpenAppCount();
}
void MainWindowBrick::handleExit() {
    qDebug() << "handleExit triggered";
    QApplication::quit();
}
void MainWindowBrick::launchInfoWindow() {}
void MainWindowBrick::activateWindow(int index) {
    interlinkBrick->activateWindow(index);
}
void MainWindowBrick::updateTaskbarWindows() {
    interlinkBrick->updateTaskbarWindows();
}
ThemeBrick* MainWindowBrick::getThemeBrick() { return themeBrick; }
InterlinkBrick* MainWindowBrick::getInterlinkBrick() { return interlinkBrick; }
