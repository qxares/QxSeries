#ifndef QXMUSICPLAYERWINDOW_H
#define QXMUSICPLAYERWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QLineEdit>
#include <QFileDialog>
#include "themebrick.h"
#include "interlinkbrick.h"

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
    InterlinkBrick *interlinkBrick;
    QMenu *fileMenu;
    QAction *darkThemeAction;
    QAction *openFileAction;
    QAction *openUrlAction;
    QAction *openDirectoryAction;
    QPushButton *playPauseButton;
    QPushButton *stopButton;
    QPushButton *nextButton;
    QPushButton *prevButton;
    QListWidget *playlistWidget;
    QLineEdit *urlBar;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;

private slots:
    void toggleDarkTheme();
    void handleWindowStateChange(bool minimized);
    void playPause();
    void stop();
    void next();
    void prev();
    void openFile();
    void openUrl();
    void openDirectory();
    void handleMediaError();
    void playlistSelectionChanged();
};

#endif // QXMUSICPLAYERWINDOW_H
