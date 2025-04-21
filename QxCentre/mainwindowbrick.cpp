#include "mainwindowbrick.h"
#include "themebrick.h"
#include "QxWrite/qxwritewindowbrick.h"
#include "QxSheet/qxsheetwindowbrick.h"
#include <QDebug>

MainWindowBrick::MainWindowBrick(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("QxCentre");
    resize(1050, 800);
    themeBrick = new ThemeBrick(qApp, this);
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
    connect(darkThemeAction, &QAction::toggled, themeBrick, &ThemeBrick::toggleDarkTheme);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit");

    appsMenu = menuBar()->addMenu("&Apps");

    QMenu *audioMenu = appsMenu->addMenu("QxAudio");
    audioMenu->addAction("QxAudio player");
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
    connect(qxWriteAction, &QAction::triggered, this, &MainWindowBrick::openQxWrite);
    connect(qxSheetAction, &QAction::triggered, this, &MainWindowBrick::openQxSheet);

    QMenu *graphicsMenu = appsMenu->addMenu("QxGraphics");
    graphicsMenu->addAction("QxDraw");
    graphicsMenu->addAction("Images");
    graphicsMenu->addAction("Photos");

    QMenu *toolsMenu = appsMenu->addMenu("Tools");
    toolsMenu->addAction("QxCalc");
    toolsMenu->addAction("QxConvert");
    toolsMenu->addAction("QxDatabase");

    helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction("About QxCentre");
    helpMenu->addAction("Documentation");
    helpMenu->addAction("Check for Updates");
}

void MainWindowBrick::setupCentralWidget() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
}

void MainWindowBrick::openQxWrite() {
    QxWriteWindowBrick *writeWindow = new QxWriteWindowBrick(this);
    writeWindow->setGeometry(0, 50, 525, 750);
    writeWindow->initializeTheme(darkThemeAction->isChecked());
    writeWindow->show();
}

void MainWindowBrick::openQxSheet() {
    QxSheetWindowBrick *sheetWindow = new QxSheetWindowBrick(this);
    sheetWindow->setGeometry(525, 50, 525, 750);
    sheetWindow->show();
}
