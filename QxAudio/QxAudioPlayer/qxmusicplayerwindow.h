#ifndef QXMUSICPLAYERWINDOW_H
#define QXMUSICPLAYERWINDOW_H

#include <QMainWindow>
#include "themebrick.h"
#include "windowmanagerbrick.h"

class QAction;
class QMenu;
class QPushButton;
class QListWidget;

class QxMusicPlayerWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit QxMusicPlayerWindow(QWidget *parent = nullptr);
    void initializeTheme(bool isDark);

private:
    void setupMenus();
    void setupCentralWidget();
    ThemeBrick *themeBrick;
    WindowManagerBrick *windowManagerBrick;
    QMenu *fileMenu;
    QAction *darkThemeAction;
    QPushButton *playPauseButton;
    QPushButton *stopButton;
    QPushButton *nextButton;
    QPushButton *prevButton;
    QListWidget *playlistWidget;
private slots:
    void toggleDarkTheme();
};

#endif // QXMUSICPLAYERWINDOW_H
