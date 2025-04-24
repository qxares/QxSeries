#ifndef MAINWINDOWBRICK_H
#define MAINWINDOWBRICK_H

#include <QMainWindow>
#include <QApplication>

class ThemeBrick;
class WindowManagerBrick;
class QxWriteWindowBrick;
class QxSheetWindowBrick;
class QxMusicPlayerWindow;

class MainWindowBrick : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindowBrick(QWidget *parent = nullptr);
    ThemeBrick* getThemeBrick() { return themeBrick; }

private slots:
    void openQxWrite();
    void openQxSheet();
    void openQxMusicPlayer();
    void toggleDarkTheme();

private:
    void setupMenus();
    void setupCentralWidget();
    ThemeBrick *themeBrick;
    WindowManagerBrick *windowManagerBrick;
    QMenu *fileMenu;
    QMenu *appsMenu;
    QMenu *helpMenu;
    QAction *darkThemeAction;
    QAction *qxWriteAction;
    QAction *qxSheetAction;
    QAction *qxMusicPlayerAction;
};

#endif // MAINWINDOWBRICK_H
