#ifndef MAINWINDOWBRICK_H
#define MAINWINDOWBRICK_H

#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include "themebrick.h"
#include "interlinkbrick.h"

class MainWindowBrick : public QWidget {
    Q_OBJECT
public:
    explicit MainWindowBrick(QWidget *parent = nullptr);
    ~MainWindowBrick();
    InterlinkBrick* getInterlinkBrick();
    ThemeBrick* getThemeBrick();

public slots:
    void raiseGroup();

private:
    void setupMenus();
    void moveToBottomLeft();
    void closeEvent(QCloseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QMenu *fileMenu;
    QMenu *appsMenu;
    QMenu *helpMenu;
    QAction *darkThemeAction;
    QAction *exitAction;
    ThemeBrick *themeBrick;
    InterlinkBrick *interlinkBrick;
    QMenuBar *menuBarWidget;
    bool isRaisingGroup;
    int openAppCount;

private slots:
    void handleAppWindowDestroyed(QObject *obj);
    void handleExit();
};

#endif // MAINWINDOWBRICK_H
