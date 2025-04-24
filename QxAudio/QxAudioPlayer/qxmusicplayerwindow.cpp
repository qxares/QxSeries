#include "qxmusicplayerwindow.h"
#include <QMenuBar>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include "mainwindowbrick.h"

QxMusicPlayerWindow::QxMusicPlayerWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("QxMusicPlayer");
    resize(400, 600);
    themeBrick = new ThemeBrick(qApp, this);
    MainWindowBrick *mainWindow = qobject_cast<MainWindowBrick*>(parent);
    if (mainWindow) {
        windowManagerBrick = new WindowManagerBrick(this, this);
        windowManagerBrick->registerAppWindow(this);
    }
    setupMenus();
    setupCentralWidget();
    qDebug() << "QxMusicPlayer main window initialized";
}

void QxMusicPlayerWindow::initializeTheme(bool isDark) {
    themeBrick->toggleDarkTheme(isDark);
    darkThemeAction->setChecked(isDark);
}

void QxMusicPlayerWindow::setupMenus() {
    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("Open File");
    fileMenu->addAction("Open Playlist");
    fileMenu->addAction("Save Playlist");
    QMenu *preferencesMenu = fileMenu->addMenu("Preferences");
    QMenu *themesMenu = preferencesMenu->addMenu("Themes");
    darkThemeAction = themesMenu->addAction("Dark");
    darkThemeAction->setCheckable(true);
    darkThemeAction->setChecked(themeBrick->isDarkTheme());
    fileMenu->addSeparator();
    fileMenu->addAction("Exit");

    connect(darkThemeAction, &QAction::triggered, this, &QxMusicPlayerWindow::toggleDarkTheme);
}

void QxMusicPlayerWindow::setupCentralWidget() {
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // Playback controls
    QWidget *controls = new QWidget(central);
    QHBoxLayout *controlLayout = new QHBoxLayout(controls);
    controlLayout->setContentsMargins(15, 0, 15, 0); // 15pt left/right margins

    // Left side: Play/Pause, Prev
    QVBoxLayout *leftLayout = new QVBoxLayout();
    playPauseButton = new QPushButton("Play/Pause", controls);
    prevButton = new QPushButton("Prev", controls);
    QSize buttonSize(100, 30);
    playPauseButton->setFixedSize(buttonSize);
    prevButton->setFixedSize(buttonSize);
    leftLayout->addWidget(playPauseButton);
    leftLayout->addSpacing(20); // 20pt vertical spacing
    leftLayout->addWidget(prevButton);
    leftLayout->addStretch(); // Push buttons up

    // Right side: Stop, Next
    QVBoxLayout *rightLayout = new QVBoxLayout();
    stopButton = new QPushButton("Stop", controls);
    nextButton = new QPushButton("Next", controls);
    stopButton->setFixedSize(buttonSize);
    nextButton->setFixedSize(buttonSize);
    rightLayout->addWidget(stopButton);
    rightLayout->addSpacing(20); // 20pt vertical spacing
    rightLayout->addWidget(nextButton);
    rightLayout->addStretch(); // Push buttons up

    controlLayout->addLayout(leftLayout);
    controlLayout->addStretch(); // Center layouts
    controlLayout->addLayout(rightLayout);
    controls->setLayout(controlLayout);

    // Playlist
    playlistWidget = new QListWidget(central);
    playlistWidget->addItem("Song 1"); // Placeholder
    playlistWidget->addItem("Song 2");
    playlistWidget->addItem("Song 3");

    mainLayout->addWidget(controls);
    mainLayout->addWidget(playlistWidget, 1); // Stretch to fill space
    central->setLayout(mainLayout);
    setCentralWidget(central);
}

void QxMusicPlayerWindow::toggleDarkTheme() {
    themeBrick->toggleDarkTheme(darkThemeAction->isChecked());
}
