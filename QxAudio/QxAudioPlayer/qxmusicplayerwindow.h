#ifndef QXMUSICPLAYERWINDOW_H
#define QXMUSICPLAYERWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QLineEdit>
#include <QFileDialog>
#include <QLabel>
#include <QSlider>
#include "themebrick.h"
#include "interlinkbrick.h"
#include "visualizerwidget.h"

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
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void setupMenus();
    void setupCentralWidget();
    void updateAlbumArt();
    void highlightCurrentTrack();
    QString getDisplayName(const QString &filePath, const QString &baseDir);
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
    QPushButton *shuffleButton;
    QPushButton *clearButton;
    QPushButton *repeatButton;
    QListWidget *playlistWidget;
    QLineEdit *urlBar;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    QLabel *albumArtLabel;
    VisualizerWidget *visualizerWidget;
    bool showingVisualizer;
    QSlider *seekSlider;
    QLabel *currentTimeLabel;
    QLabel *totalTimeLabel;

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
    void playSelectedTrack();
    void toggleVisualizer();
    void updateSeekSlider(qint64 position);
    void updateDuration(qint64 duration);
    void seekPosition(int value);
};

#endif // QXMUSICPLAYERWINDOW_H
