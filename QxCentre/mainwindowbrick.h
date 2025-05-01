#ifndef MAINWINDOWBRICK_H
#define MAINWINDOWBRICK_H

#include <QMainWindow>
#include <QDockWidget>
#include <QToolBar>
#include <QMenu>
#include <QComboBox>
#include <QAction>
#include <QPointer>
#include "themebrick.h"
#include "interlinkbrick.h"

class MainWindowBrick : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindowBrick(QWidget *parent = nullptr);
    ~MainWindowBrick();
    ThemeBrick* getThemeBrick();
    InterlinkBrick* getInterlinkBrick();

protected:
    void moveEvent(QMoveEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void handleAppWindowDestroyed(QObject *obj);
    void handleExit();
    void launchApp(QAction *action);
    void activateWindow(int index);
    void updateTaskbarWindows();

private:
    void setupTaskbar();
    void centerWindow();
    void raiseGroup();

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
