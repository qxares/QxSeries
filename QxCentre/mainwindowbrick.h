#ifndef MAINWINDOWBRICK_H
#define MAINWINDOWBRICK_H

#include <QMainWindow>
#include <QDockWidget>
#include <QToolBar>
#include <QComboBox>
#include <QMenu>
#include <QAction>
#include "themebrick.h"

class InterlinkBrick; // Forward declaration

class MainWindowBrick : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindowBrick(QWidget *parent = nullptr);
    ~MainWindowBrick();

    ThemeBrick* getThemeBrick();
    InterlinkBrick* getInterlinkBrick();
    void handleAppWindowDestroyed(QObject *obj);
    void incrementOpenAppCount();

protected:
    void closeEvent(QCloseEvent *event) override;
    void moveEvent(QMoveEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void handleExit();
    void launchInfoWindow();
    void activateWindow(int index);
    void updateTaskbarWindows();

private:
    void setupTaskbar();
    void centerWindow();
    void raiseGroup();
    void decrementOpenAppCount();

    ThemeBrick *themeBrick;
    InterlinkBrick *interlinkBrick;
    QDockWidget *taskbarDock;
    QToolBar *taskbar;
    QMenu *qxCentreMenu;
    QMenu *qxAppsMenu;
    QMenu *helpMenu;
    QAction *exitAction;
    QComboBox *windowList;
    bool isRaisingGroup;
    int openAppCount;
};

#endif // MAINWINDOWBRICK_H
