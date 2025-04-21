#ifndef MAINWINDOWBRICK_H
#define MAINWINDOWBRICK_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QWidget>
#include <QAction>
#include "themebrick.h"

class MainWindowBrick : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindowBrick(QWidget *parent = nullptr);
    ThemeBrick* getThemeBrick() const { return themeBrick; }

private slots:
    void openQxWrite();
    void openQxSheet();

private:
    void setupMenus();
    void setupCentralWidget();

    QMenu *fileMenu;
    QMenu *appsMenu;
    QMenu *helpMenu;
    QWidget *centralWidget;
    QAction *darkThemeAction;
    QAction *qxWriteAction;
    QAction *qxSheetAction;
    ThemeBrick *themeBrick;
};

#endif // MAINWINDOWBRICK_H
