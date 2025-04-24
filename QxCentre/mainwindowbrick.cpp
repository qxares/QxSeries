#include "mainwindowbrick.h"
#include <QMenuBar>
#include <QDebug>
#include <QMessageBox>
#include "interlinkbrick.h"
#include "../QxText/QxWrite/qxwritewindowbrick.h"
#include "../QxText/QxSheet/qxsheetwindowbrick.h"
#include "../QxAudio/QxAudioPlayer/qxmusicplayerwindow.h"
#include "themebrick.h"

MainWindowBrick::MainWindowBrick(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("QxCentre");
    resize(1050, 800);
    themeBrick = new ThemeBrick(qApp, this);
    interlinkBrick = new InterlinkBrick(this, this);
    setupMenus();
    setupCentralWidget();
    qDebug() << "QxCentre main window initialized";
}

void MainWindowBrick::setupMenus() {
    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("New Database");
    fileMenu->addAction("Open Database");
    fileMenu->addAction("Save Database");
    fileMenu->addAction("Save As");
    QMenu *preferencesMenu = fileMenu->addMenu("Preferences");
    QMenu *themesMenu = preferencesMenu->addMenu("Themes");
    darkThemeAction = themesMenu->addAction("Dark");
    darkThemeAction->setCheckable(true);
    darkThemeAction->setChecked(themeBrick->isDarkTheme());
    fileMenu->addSeparator();
    fileMenu->addAction("Exit");

    appsMenu = menuBar()->addMenu("&Apps");
    QMenu *audioMenu = appsMenu->addMenu("QxAudio");
    qxMusicPlayerAction = audioMenu->addAction("QxMusicPlayer");
    audioMenu->addAction("Music");
    audioMenu->addAction("Books");
    audioMenu->addAction("Recordings");

    QMenu *videoMenu = appsMenu->addMenu("QxVideo");
    videoMenu->addAction("QxVideo player");
    videoMenu->addAction("Movies");
    videoMenu->addAction("Series");
    videoMenu->addAction("Recordings");

    QMenu *textMenu = appsMenu->addMenu("QxText");
    qxWriteAction = textMenu->addAction("QxWrite");
    qxSheetAction = textMenu->addAction("QxSheet");
    textMenu->addAction("QxNotes");

    QMenu *graphicsMenu = appsMenu->addMenu("QxGraphics");
    graphicsMenu->addAction("QxDraw");
    graphicsMenu->addAction("Images");
    graphicsMenu->addAction("Photos");

    QMenu *toolsMenu = appsMenu->addMenu("Tools");
    toolsMenu->addAction("QxCalc");
    toolsMenu->addAction("QxConvert");

    helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction("About QxCentre");
    helpMenu->addAction("Documentation");
    helpMenu->addAction("Check for Updates");

    connect(qxWriteAction, &QAction::triggered, this, &MainWindowBrick::openQxWrite);
    connect(qxSheetAction, &QAction::triggered, this, &MainWindowBrick::openQxSheet);
    connect(qxMusicPlayerAction, &QAction::triggered, this, &MainWindowBrick::openQxMusicPlayer);
    connect(darkThemeAction, &QAction::triggered, this, &MainWindowBrick::toggleDarkTheme);
}

void MainWindowBrick::openQxWrite() {
    QxWriteWindowBrick *writeWindow = new QxWriteWindowBrick(this);
    writeWindow->setAttribute(Qt::WA_DeleteOnClose);
    writeWindow->move(0, 50);
    writeWindow->resize(525, 750);
    writeWindow->initializeTheme(themeBrick->isDarkTheme());
    interlinkBrick->launchAppWindow(writeWindow);
    qDebug() << "QxWrite window opened";
}

void MainWindowBrick::openQxSheet() {
    QxSheetWindowBrick *sheetWindow = new QxSheetWindowBrick(this);
    sheetWindow->setAttribute(Qt::WA_DeleteOnClose);
    sheetWindow->move(0, 50);
    sheetWindow->resize(525, 750);
    sheetWindow->initializeTheme(themeBrick->isDarkTheme());
    interlinkBrick->launchAppWindow(sheetWindow);
    qDebug() << "QxSheet window opened";
}

void MainWindowBrick::openQxMusicPlayer() {
    QxMusicPlayerWindow *musicWindow = new QxMusicPlayerWindow(this);
    musicWindow->setAttribute(Qt::WA_DeleteOnClose);
    musicWindow->move(0, 50);
    musicWindow->resize(400, 600);
    musicWindow->initializeTheme(themeBrick->isDarkTheme());
    interlinkBrick->launchAppWindow(musicWindow);
    qDebug() << "QxMusicPlayer window opened";
}

void MainWindowBrick::toggleDarkTheme() {
    themeBrick->toggleDarkTheme(darkThemeAction->isChecked());
}

void MainWindowBrick::setupCentralWidget() {
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
}
