#include "qxmusicplayerwindow.h"
#include <QMenuBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QFileDialog>
#include <QDir>
#include <QApplication>
#include <QFont>
#include <QResizeEvent>
#include "../../QxCentre/mainwindowbrick.h"

QxMusicPlayerWindow::QxMusicPlayerWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("QxMusicPlayer");
    resize(400, 600);
    themeBrick = new ThemeBrick(qApp, this);
    settings = new QSettings("QxSeries", "QxMusicPlayer", this);
    MainWindowBrick *mainWindow = qobject_cast<MainWindowBrick*>(parent);
    if (mainWindow) {
        interlinkBrick = mainWindow->getInterlinkBrick();
        if (interlinkBrick) {
            interlinkBrick->registerAppWindow(this);
            connect(interlinkBrick, &InterlinkBrick::windowStateChanged, this, &QxMusicPlayerWindow::handleWindowStateChange);
        }
    }
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    playlist->setPlaybackMode(QMediaPlaylist::Sequential);
    player->setPlaylist(playlist);
    player->setNotifyInterval(1000);
    connect(player, static_cast<void(QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error),
            this, &QxMusicPlayerWindow::handleMediaError);
    connect(player, &QMediaPlayer::positionChanged, this, &QxMusicPlayerWindow::updateSeekSlider);
    connect(player, &QMediaPlayer::durationChanged, this, &QxMusicPlayerWindow::updateDuration);
    connect(playlist, &QMediaPlaylist::currentIndexChanged, this, &QxMusicPlayerWindow::highlightCurrentTrack);
    isShuffled = false;
    repeatMode = QMediaPlaylist::Sequential;
    setupMenus();
    setupCentralWidget();
}

void QxMusicPlayerWindow::initializeTheme(bool isDark) {
    themeBrick->toggleDarkTheme(isDark);
    darkThemeAction->setChecked(isDark);
    QPalette palette = qApp->palette();
    currentTimeLabel->setPalette(palette);
    totalTimeLabel->setPalette(palette);
    shuffleButton->setPalette(palette);
    clearButton->setPalette(palette);
    repeatButton->setPalette(palette);
    volumeUpButton->setPalette(palette);
    muteButton->setPalette(palette);
    volumeDownButton->setPalette(palette);
    QApplication::setStyle("fusion");
}

void QxMusicPlayerWindow::setupMenus() {
    fileMenu = menuBar()->addMenu("&File");
    openFileAction = fileMenu->addAction("Open File");
    openDirectoryAction = fileMenu->addAction("Open Directory");
    fileMenu->addAction("Open Playlist");
    fileMenu->addAction("Save Playlist");
    QMenu *preferencesMenu = fileMenu->addMenu("Preferences");
    QMenu *themesMenu = preferencesMenu->addMenu("Themes");
    darkThemeAction = themesMenu->addAction("Dark");
    darkThemeAction->setCheckable(true);
    darkThemeAction->setChecked(themeBrick->isDarkTheme());
    fileMenu->addSeparator();
    fileMenu->addAction("Exit");

    connect(darkThemeAction, &QAction::triggered, this, &QxMusicPlayerWindow::toggleDarkTheme);
    connect(openFileAction, &QAction::triggered, this, &QxMusicPlayerWindow::openFile);
    connect(openDirectoryAction, &QAction::triggered, this, &QxMusicPlayerWindow::openDirectory);
}

void QxMusicPlayerWindow::setupCentralWidget() {
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(0);

    QWidget *controls = new QWidget(central);
    QHBoxLayout *controlLayout = new QHBoxLayout(controls);
    controlLayout->setContentsMargins(5, 20, 5, 0);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    playPauseButton = new QPushButton("Play/Pause", controls);
    prevButton = new QPushButton("Prev", controls);
    QSize buttonSize(100, 30);
    playPauseButton->setFixedSize(buttonSize);
    prevButton->setFixedSize(buttonSize);
    leftLayout->addWidget(playPauseButton);
    leftLayout->addSpacing(20);
    leftLayout->addWidget(prevButton);

    QVBoxLayout *centerLayout = new QVBoxLayout();
    centerLayout->setAlignment(Qt::AlignCenter);
    QHBoxLayout *extraButtonsLayout = new QHBoxLayout();
    extraButtonsLayout->setSpacing(10);
    volumeUpButton = new QPushButton("+", controls);
    muteButton = new QPushButton("Mute", controls);
    volumeDownButton = new QPushButton("-", controls);
    QSize smallButtonSize(60, 20);
    QFont textFont("Arial", 10);
    QFont symbolFont("Arial", 12);
    volumeUpButton->setFixedSize(smallButtonSize);
    muteButton->setFixedSize(smallButtonSize);
    volumeDownButton->setFixedSize(smallButtonSize);
    muteButton->setFont(textFont);
    volumeUpButton->setFont(symbolFont);
    volumeDownButton->setFont(symbolFont);
    extraButtonsLayout->addWidget(volumeUpButton);
    extraButtonsLayout->addWidget(muteButton);
    extraButtonsLayout->addWidget(volumeDownButton);
    centerLayout->addLayout(extraButtonsLayout);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    stopButton = new QPushButton("Stop", controls);
    nextButton = new QPushButton("Next", controls);
    stopButton->setFixedSize(buttonSize);
    nextButton->setFixedSize(buttonSize);
    rightLayout->addWidget(stopButton);
    rightLayout->addSpacing(20);
    rightLayout->addWidget(nextButton);

    controlLayout->addLayout(leftLayout);
    controlLayout->addStretch();
    controlLayout->addLayout(centerLayout);
    controlLayout->addStretch();
    controlLayout->addLayout(rightLayout);
    mainLayout->addWidget(controls);
    mainLayout->addSpacing(40);

    QHBoxLayout *timeLayout = new QHBoxLayout();
    seekSlider = new QSlider(Qt::Horizontal, central);
    currentTimeLabel = new QLabel("0:00", central);
    totalTimeLabel = new QLabel("0:00", central);
    timeLayout->addWidget(currentTimeLabel);
    timeLayout->addWidget(seekSlider);
    timeLayout->addWidget(totalTimeLabel);
    mainLayout->addLayout(timeLayout);
    mainLayout->addSpacing(20);

    playlistWidget = new QListWidget(central);
    mainLayout->addWidget(playlistWidget, 1);
    mainLayout->addSpacing(15);

    QHBoxLayout *playlistControlsLayout = new QHBoxLayout();
    playlistControlsLayout->setSpacing(10);
    shuffleButton = new QPushButton("Shuffle", central);
    clearButton = new QPushButton("Clear", central);
    repeatButton = new QPushButton("Repeat", central);
    shuffleButton->setFixedSize(smallButtonSize);
    clearButton->setFixedSize(smallButtonSize);
    repeatButton->setFixedSize(smallButtonSize);
    shuffleButton->setFont(textFont);
    clearButton->setFont(textFont);
    repeatButton->setFont(textFont);
    playlistControlsLayout->addStretch();
    playlistControlsLayout->addWidget(shuffleButton);
    playlistControlsLayout->addWidget(clearButton);
    playlistControlsLayout->addWidget(repeatButton);
    playlistControlsLayout->addStretch();
    mainLayout->addLayout(playlistControlsLayout);

    setCentralWidget(central);

    connect(playPauseButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::playPause);
    connect(stopButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::stop);
    connect(nextButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::next);
    connect(prevButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::prev);
    connect(shuffleButton, &QPushButton::clicked, this, [this] { toggleShuffle(!isShuffled); });
    connect(clearButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::clearPlaylist);
    connect(repeatButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::toggleRepeat);
    connect(volumeUpButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::volumeUp);
    connect(muteButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::toggleMute);
    connect(volumeDownButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::volumeDown);
    connect(seekSlider, &QSlider::sliderMoved, this, &QxMusicPlayerWindow::seekPosition);
    connect(playlistWidget, &QListWidget::itemSelectionChanged, this, &QxMusicPlayerWindow::playlistSelectionChanged);
    connect(playlistWidget, &QListWidget::itemDoubleClicked, this, &QxMusicPlayerWindow::playSelectedTrack);

    playlistWidget->setMinimumWidth(300);
    seekSlider->setMinimumWidth(playlistWidget->width());
}

void QxMusicPlayerWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    seekSlider->setMinimumWidth(playlistWidget->width());
}

void QxMusicPlayerWindow::toggleDarkTheme() {
    bool isDark = darkThemeAction->isChecked();
    initializeTheme(isDark);
}

void QxMusicPlayerWindow::handleWindowStateChange(bool minimized) {
    if (minimized) {
        showMinimized();
    } else {
        showNormal();
    }
}

void QxMusicPlayerWindow::playPause() {
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
    } else {
        player->play();
    }
}

void QxMusicPlayerWindow::stop() {
    player->stop();
}

void QxMusicPlayerWindow::next() {
    playlist->next();
}

void QxMusicPlayerWindow::prev() {
    playlist->previous();
}

void QxMusicPlayerWindow::toggleShuffle(bool checked) {
    isShuffled = checked;
    shuffleButton->setText(isShuffled ? "Shuffle On" : "Shuffle");
    if (isShuffled) {
        playlist->shuffle();
        playlistWidget->clear();
        for (int i = 0; i < playlist->mediaCount(); ++i) {
            QMediaContent media = playlist->media(i);
            QString displayName = media.request().url().toString().startsWith("file://") ?
                getDisplayName(media.request().url().toLocalFile()) : media.request().url().toString();
            playlistWidget->addItem(displayName);
        }
        playlist->setPlaybackMode(QMediaPlaylist::Sequential);
    } else {
        playlistWidget->clear();
        playlist->clear();
        for (const QString &file : originalPlaylistOrder) {
            QUrl url = file.startsWith("http") ? QUrl(file) : QUrl::fromLocalFile(file);
            playlist->addMedia(url);
            playlistWidget->addItem(getDisplayName(file));
        }
        playlist->setPlaybackMode(repeatMode);
    }
}

void QxMusicPlayerWindow::clearPlaylist() {
    playlist->clear();
    playlistWidget->clear();
    originalPlaylistOrder.clear();
    player->stop();
}

void QxMusicPlayerWindow::toggleRepeat() {
    if (repeatMode == QMediaPlaylist::Sequential) {
        repeatMode = QMediaPlaylist::Loop;
        repeatButton->setText("Repeat All");
    } else if (repeatMode == QMediaPlaylist::Loop) {
        repeatMode = QMediaPlaylist::CurrentItemInLoop;
        repeatButton->setText("Repeat One");
    } else {
        repeatMode = QMediaPlaylist::Sequential;
        repeatButton->setText("Repeat");
    }
    if (!isShuffled) {
        playlist->setPlaybackMode(repeatMode);
    }
}

void QxMusicPlayerWindow::volumeUp() {
    int currentVolume = player->volume();
    player->setVolume(qMin(currentVolume + 10, 100));
}

void QxMusicPlayerWindow::toggleMute() {
    bool isMuted = player->isMuted();
    player->setMuted(!isMuted);
    muteButton->setText(isMuted ? "Mute" : "Unmute");
}

void QxMusicPlayerWindow::volumeDown() {
    int currentVolume = player->volume();
    player->setVolume(qMax(currentVolume - 10, 0));
}

QString QxMusicPlayerWindow::getDisplayName(const QString &filePath) {
    return QFileInfo(filePath).fileName();
}

void QxMusicPlayerWindow::openFile() {
    QFileDialog dialog(this, "Open Audio Files", QDir::homePath(), "Audio Files (*.mp3 *.flac *.wav)");
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);
    QStringList files = dialog.getOpenFileNames();
    QStringList displayNames;
    for (const QString &file : files) {
        playlist->addMedia(QUrl::fromLocalFile(file));
        displayNames.append(getDisplayName(file));
        originalPlaylistOrder.append(file);
    }
    playlistWidget->addItems(displayNames);
}

void QxMusicPlayerWindow::openDirectory() {
    QFileDialog dialog(this, "Select Directory", QDir::homePath());
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);
    dialog.setFileMode(QFileDialog::Directory);
    if (dialog.exec()) {
        QString dirPath = dialog.selectedFiles().first();
        QDir dir(dirPath);
        QStringList nameFilters = {"*.mp3", "*.flac", "*.wav"};
        dir.setNameFilters(nameFilters);
        dir.setFilter(QDir::Files | QDir::Readable);
        QFileInfoList fileInfoList = dir.entryInfoList();
        QStringList displayNames;
        for (const QFileInfo &fileInfo : fileInfoList) {
            QString filePath = fileInfo.absoluteFilePath();
            playlist->addMedia(QUrl::fromLocalFile(filePath));
            displayNames.append(getDisplayName(filePath));
            originalPlaylistOrder.append(filePath);
        }
        playlistWidget->addItems(displayNames);
    }
}

void QxMusicPlayerWindow::handleMediaError() {
    qWarning() << "Media error:" << player->errorString();
}

void QxMusicPlayerWindow::playlistSelectionChanged() {
    int index = playlistWidget->currentRow();
    if (index >= 0) {
        playlist->setCurrentIndex(index);
    }
}

void QxMusicPlayerWindow::playSelectedTrack() {
    int index = playlistWidget->currentRow();
    if (index >= 0) {
        playlist->setCurrentIndex(index);
        player->play();
    }
}

void QxMusicPlayerWindow::highlightCurrentTrack() {
    int index = playlist->currentIndex();
    if (index >= 0 && index < playlistWidget->count()) {
        playlistWidget->setCurrentRow(index);
    }
}

void QxMusicPlayerWindow::updateSeekSlider(qint64 position) {
    seekSlider->setMaximum(player->duration());
    seekSlider->setValue(position);
    int seconds = (position / 1000) % 60;
    int minutes = (position / 60000);
    currentTimeLabel->setText(QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0')));
}

void QxMusicPlayerWindow::updateDuration(qint64 duration) {
    int seconds = (duration / 1000) % 60;
    int minutes = (duration / 60000);
    totalTimeLabel->setText(QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0')));
}

void QxMusicPlayerWindow::seekPosition(int value) {
    player->setPosition(value);
}
