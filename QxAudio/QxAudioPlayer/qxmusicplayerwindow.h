#ifndef QXMUSICPLAYERWINDOW_H
#define QXMUSICPLAYERWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QLabel>
#include <QSlider>
#include <QHash>
#include <QSettings>
#include "../../QxCentre/themebrick.h"
#include "../../QxCentre/interlinkbrick.h"

class QAction;
class QMenu;
class QPushButton;
class QListWidget;

class QxMusicPlayerWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit QxMusicPlayerWindow(QWidget *parent = nullptr);
    void initializeTheme(bool isDark);
protected:
    void resizeEvent(QResizeEvent *event) override;
private:
    void setupMenus();
    void setupCentralWidget();
    void highlightCurrentTrack();
    QString getDisplayName(const QString &filePath);
    ThemeBrick *themeBrick;
    InterlinkBrick *interlinkBrick;
    QMenu *fileMenu;
    QAction *darkThemeAction;
    QAction *openFileAction;
    QAction *openDirectoryAction;
    QPushButton *playPauseButton;
    QPushButton *stopButton;
    QPushButton *nextButton;
    QPushButton *prevButton;
    QPushButton *shuffleButton;
    QPushButton *clearButton;
    QPushButton *repeatButton;
    QPushButton *volumeUpButton;
    QPushButton *muteButton;
    QPushButton *volumeDownButton;
    QListWidget *playlistWidget;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    QSlider *seekSlider;
    QLabel *currentTimeLabel;
    QLabel *totalTimeLabel;
    QHash<QString, QStringList> fileCache;
    QSettings *settings;
    QString currentDirPath;
    bool isShuffled;
    QMediaPlaylist::PlaybackMode repeatMode;
    QStringList originalPlaylistOrder;
private slots:
    void toggleDarkTheme();
    void handleWindowStateChange(bool minimized);
    void playPause();
    void stop();
    void next();
    void prev();
    void toggleShuffle(bool checked);
    void clearPlaylist();
    void toggleRepeat();
    void volumeUp();
    void toggleMute();
    void volumeDown();
    void openFile();
    void openDirectory();
    void handleMediaError();
    void playlistSelectionChanged();
    void playSelectedTrack();
    void updateSeekSlider(qint64 position);
    void updateDuration(qint64 duration);
    void seekPosition(int value);
};

#endif // QXMUSICPLAYERWINDOW_H
