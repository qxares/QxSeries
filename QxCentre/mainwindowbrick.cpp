#include "mainwindowbrick.h"
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QScreen>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QVBoxLayout>

MainWindowBrick::MainWindowBrick(QWidget *parent) : QWidget(parent) {
    themeBrick = new ThemeBrick(qApp, this);
    interlinkBrick = new InterlinkBrick(this);
    isRaisingGroup = false;
    openAppCount = 0;
    
    setWindowFlags(Qt::Window | Qt::WindowStaysOnBottomHint);
    setWindowTitle("QxCentre");
    resize(1050, 800);
    setMinimumSize(600, 400);
    setAttribute(Qt::WA_QuitOnClose, false);

    menuBarWidget = new QMenuBar(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(menuBarWidget);
    setLayout(layout);

    setupMenus();
    moveToBottomLeft();
    qDebug() << "QxCentre main window initialized";
}

MainWindowBrick::~MainWindowBrick() {
    if (menuBarWidget) {
        delete menuBarWidget;
        menuBarWidget = nullptr;
        qDebug() << "Deleted menuBarWidget in destructor";
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
    qDebug() << "MainWindowBrick destroyed";
}

void MainWindowBrick::setupMenus() {
    fileMenu = menuBarWidget->addMenu("&File");
    fileMenu->addAction("New Database");
    fileMenu->addAction("Open Database");
    fileMenu->addAction("Save Database");
    fileMenu->addAction("Save As");
    QMenu *preferencesMenu = fileMenu->addMenu("Preferences");
    QMenu *themesMenu = preferencesMenu->addMenu("Themes");
    darkThemeAction = themesMenu->addAction("Dark");
    QAction *lightThemeAction = themesMenu->addAction("Light");
    fileMenu->addSeparator();
    exitAction = fileMenu->addAction("Exit");

    appsMenu = menuBarWidget->addMenu("&Apps");
    QMenu *audioMenu = appsMenu->addMenu("QxAudio");
    audioMenu->addAction("Music");
    audioMenu->addAction("Books");
    audioMenu->addAction("Recordings");
    QMenu *videoMenu = appsMenu->addMenu("QxVideo");
    videoMenu->addAction("QxVideo player");
    videoMenu->addAction("Movies");
    videoMenu->addAction("Series");
    videoMenu->addAction("Recordings");
    QMenu *documentMenu = appsMenu->addMenu("QxDocument");
    documentMenu->addAction("QxNotes");
    documentMenu->addAction("QxWrite");
    documentMenu->addAction("QxSheet");
    QMenu *graphicsMenu = appsMenu->addMenu("QxGraphics");
    graphicsMenu->addAction("QxDraw");
    graphicsMenu->addAction("Images");
    graphicsMenu->addAction("Photos");
    QMenu *toolsMenu = appsMenu->addMenu("QxTools");
    toolsMenu->addAction("QxCalc");
    toolsMenu->addAction("QxConvert");

    helpMenu = menuBarWidget->addMenu("&Help");
    helpMenu->addAction("About QxCentre");
    helpMenu->addAction("Documentation");
    helpMenu->addAction("Check for Updates");

    connect(darkThemeAction, &QAction::triggered, this, [this]() { themeBrick->toggleDarkTheme(true); });
    connect(lightThemeAction, &QAction::triggered, this, [this]() { themeBrick->toggleDarkTheme(false); });
    connect(exitAction, &QAction::triggered, this, &MainWindowBrick::handleExit);
}

void MainWindowBrick::moveToBottomLeft() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = 0;
    int y = screenGeometry.height() - height();
    move(x, y);
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
    QWidget::mousePressEvent(event);
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

ThemeBrick* MainWindowBrick::getThemeBrick() {
    return themeBrick;
}

InterlinkBrick* MainWindowBrick::getInterlinkBrick() {
    return interlinkBrick;
}
