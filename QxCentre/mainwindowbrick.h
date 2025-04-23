#ifndef MAINWINDOWBRICK_H
#define MAINWINDOWBRICK_H

#include <QMainWindow>
#include <QApplication>

class ThemeBrick;
class QxWriteWindowBrick;
class QxSheetWindowBrick;

class MainWindowBrick : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindowBrick(QWidget *parent = nullptr);
    ThemeBrick* getThemeBrick() { return themeBrick; }

private slots:
    void openQxWrite();
    void openQxSheet();
    void toggleDarkTheme();

private:
    void setupMenus();
    void setupCentralWidget();
    ThemeBrick *themeBrick;
    QMenu *fileMenu;
    QMenu *appsMenu;
    QMenu *helpMenu;
    QAction *darkThemeAction;
    QAction *qxWriteAction;
    QAction *qxSheetAction;
};

#endif // MAINWINDOWBRICK_H
