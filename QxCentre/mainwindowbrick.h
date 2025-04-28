#ifndef MAINWINDOWBRICK_H
#define MAINWINDOWBRICK_H

#include <QMainWindow>
#include <QDockWidget>
#include <QToolBar>
#include <QComboBox>
#include <QMenu>
#include <QMessageBox>
#include "themebrick.h"
#include "interlinkbrick.h"

class MainWindowBrick : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindowBrick(QWidget *parent = nullptr);
    ~MainWindowBrick();
    InterlinkBrick* getInterlinkBrick();
    ThemeBrick* getThemeBrick();

public slots:
    void raiseGroup();
    void updateTaskbarWindows();

private:
    void setupTaskbar();
    void moveToBottomLeft();
    void closeEvent(QCloseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QDockWidget *taskbarDock;
    QToolBar *taskbar;
    QComboBox *windowList;
    QMenu *qxCentreMenu;
    QMenu *qxAppsMenu;
    QMenu *helpMenu;
    QAction *exitAction;
    ThemeBrick *themeBrick;
    InterlinkBrick *interlinkBrick;
    bool isRaisingGroup;
    int openAppCount;

private slots:
    void handleAppWindowDestroyed(QObject *obj);
    void handleExit();
    void launchApp(QAction *action);
    void activateWindow(int index);
    void showAboutDialog();
};

#endif // MAINWINDOWBRICK_H
