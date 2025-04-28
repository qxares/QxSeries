#include "mainwindowbrick.h"
#include <QDebug>
#include <QAction>
#include <QApplication>
#include <QScreen>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QPushButton>

MainWindowBrick::MainWindowBrick(QWidget *parent) : QMainWindow(parent) {
    themeBrick = new ThemeBrick(qApp, this);
    interlinkBrick = new InterlinkBrick(this);
    isRaisingGroup = false;
    openAppCount = 0;
    
    setWindowFlags(Qt::Window | Qt::WindowStaysOnBottomHint);
    setWindowTitle("QxCentre");
    resize(1050, 800);
    setMinimumSize(600, 400);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupTaskbar();
    moveToBottomLeft();
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
    qDebug() << "MainWindowBrick destroyed";
}

void MainWindowBrick::setupTaskbar() {
    taskbarDock = new QDockWidget(this);
    taskbarDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    taskbarDock->setAllowedAreas(Qt::TopDockWidgetArea);
    taskbarDock->setTitleBarWidget(new QWidget()); // Hide title bar
    setCentralWidget(new QWidget()); // Placeholder central widget
    addDockWidget(Qt::TopDockWidgetArea, taskbarDock);

    taskbar = new QToolBar(taskbarDock);
    taskbar->setMovable(false);
    taskbarDock->setWidget(taskbar);

    systemMenu = new QMenu("System", taskbar);
    darkThemeAction = systemMenu->addAction("Dark Theme");
    darkThemeAction->setCheckable(true);
    systemMenu->addSeparator();
    exitAction = systemMenu->addAction("Exit");
    QPushButton *systemButton = new QPushButton("System", taskbar);
    systemButton->setMenu(systemMenu);
    taskbar->addWidget(systemButton);

    QAction *writeAction = taskbar->addAction("QxWrite");
    QAction *calcAction = taskbar->addAction("QxCalc");
    taskbar->addSeparator();

    windowList = new QComboBox(taskbar);
    windowList->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    taskbar->addWidget(windowList);

    connect(darkThemeAction, &QAction::toggled, this, [this](bool checked) { themeBrick->toggleDarkTheme(checked); });
    connect(exitAction, &QAction::triggered, this, &MainWindowBrick::handleExit);
    connect(writeAction, &QAction::triggered, this, [this]() { launchApp("QxWrite"); });
    connect(calcAction, &QAction::triggered, this, [this]() { launchApp("QxCalc"); });
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

void MainWindowBrick::launchApp(const QString &appName) {
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
    windowList->clear();
    auto appWindows = interlinkBrick->getAppWindows();
    for (auto it = appWindows.constBegin(); it != appWindows.constEnd(); ++it) {
        if (it.value() && it.value()->isVisible()) {
            windowList->addItem(it.key());
        }
    }
    qDebug() << "Updated taskbar window list";
}

ThemeBrick* MainWindowBrick::getThemeBrick() {
    return themeBrick;
}

InterlinkBrick* MainWindowBrick::getInterlinkBrick() {
    return interlinkBrick;
}
