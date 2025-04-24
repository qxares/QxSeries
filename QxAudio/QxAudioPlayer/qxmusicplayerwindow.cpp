#include "qxmusicplayerwindow.h"
#include <QMenuBar>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QFileDialog>
#include <QDirIterator>
#include <QImage>
#include <QPixmap>
#include <QEvent>
#include <QDir>
#include <QTime>
#include "mainwindowbrick.h"

QxMusicPlayerWindow::QxMusicPlayerWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("QxMusicPlayer");
    resize(400, 600);
    themeBrick = new ThemeBrick(qApp, this);
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
    player->setPlaylist(playlist);
    connect(player, static_cast<void(QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error),
            this, &QxMusicPlayerWindow::handleMediaError);
    connect(player, &QMediaPlayer::positionChanged, this, &QxMusicPlayerWindow::updateSeekSlider);
    connect(player, &QMediaPlayer::durationChanged, this, &QxMusicPlayerWindow::updateDuration);
    connect(player, &QMediaPlayer::mediaChanged, this, &QxMusicPlayerWindow::updateAlbumArt);
    connect(playlist, &QMediaPlaylist::currentIndexChanged, this, &QxMusicPlayerWindow::highlightCurrentTrack);
    showingVisualizer = false;
    setupMenus();
    setupCentralWidget();
    qDebug() << "QxMusicPlayer main window initialized";
}

bool QxMusicPlayerWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == albumArtLabel && event->type() == QEvent::MouseButtonDblClick) {
        toggleVisualizer();
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

void QxMusicPlayerWindow::initializeTheme(bool isDark) {
    themeBrick->toggleDarkTheme(isDark);
    darkThemeAction->setChecked(isDark);
    QPalette palette = qApp->palette();
    urlBar->setPalette(palette);
    albumArtLabel->setPalette(palette);
    visualizerWidget->setThemeColors(
        isDark ? QColor("#00BFFF") : QColor("#FF4500"), // Neon blue (dark) or orange (light)
        palette.color(QPalette::Window)
    );
    currentTimeLabel->setPalette(palette);
    totalTimeLabel->setPalette(palette);
    shuffleButton->setPalette(palette);
    clearButton->setPalette(palette);
    repeatButton->setPalette(palette);
}

void QxMusicPlayerWindow::setupMenus() {
    fileMenu = menuBar()->addMenu("&File");
    openFileAction = fileMenu->addAction("Open File");
    openDirectoryAction = fileMenu->addAction("Open Directory");
    openUrlAction = fileMenu->addAction("Open URL");
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
    connect(openUrlAction, &QAction::triggered, this, &QxMusicPlayerWindow::openUrl);
}

void QxMusicPlayerWindow::setupCentralWidget() {
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // URL bar
    urlBar = new QLineEdit(central);
    urlBar->setPlaceholderText("Enter URL (e.g., http://stream.example.com)");
    urlBar->setFixedHeight(30);
    mainLayout->addWidget(urlBar);

    // Playback controls
    QWidget *controls = new QWidget(central);
    QHBoxLayout *controlLayout = new QHBoxLayout(controls);
    controlLayout->setContentsMargins(5, 0, 5, 0); // Reduced margins

    // Left side: Play/Pause, Prev
    QVBoxLayout *leftLayout = new QVBoxLayout();
    playPauseButton = new QPushButton("Play/Pause", controls);
    prevButton = new QPushButton("Prev", controls);
    QSize buttonSize(100, 30);
    playPauseButton->setFixedSize(buttonSize);
    prevButton->setFixedSize(buttonSize);
    leftLayout->addWidget(playPauseButton);
    leftLayout->addSpacing(10); // Reduced spacing
    leftLayout->addWidget(prevButton);

    // Center: Album art/visualization + time bar + new buttons
    QVBoxLayout *centerLayout = new QVBoxLayout();
    albumArtLabel = new QLabel(controls);
    albumArtLabel->setFixedSize(100, 100);
    albumArtLabel->setAlignment(Qt::AlignCenter);
    albumArtLabel->setStyleSheet("background-color: gray; color: white;");
    albumArtLabel->setText("No Art");
    albumArtLabel->installEventFilter(this);
    visualizerWidget = new VisualizerWidget(controls);
    visualizerWidget->hide();
    connect(visualizerWidget, &VisualizerWidget::mouseDoubleClicked, this, &QxMusicPlayerWindow::toggleVisualizer);

    // Time bar
    seekSlider = new QSlider(Qt::Horizontal, controls);
    seekSlider->setFixedWidth(200);
    currentTimeLabel = new QLabel("0:00", controls);
    totalTimeLabel = new QLabel("0:00", controls);
    QHBoxLayout *timeLayout = new QHBoxLayout();
    timeLayout->addWidget(currentTimeLabel);
    timeLayout->addWidget(seekSlider);
    timeLayout->addWidget(totalTimeLabel);

    // New buttons: Shuffle, Clear, Repeat
    QHBoxLayout *extraButtonsLayout = new QHBoxLayout();
    shuffleButton = new QPushButton("Shuffle", controls);
    clearButton = new QPushButton("Clear", controls);
    repeatButton = new QPushButton("Repeat", controls);
    QSize smallButtonSize(50, 15); // Half the size of playPauseButton
    shuffleButton->setFixedSize(smallButtonSize);
    clearButton->setFixedSize(smallButtonSize);
    repeatButton->setFixedSize(smallButtonSize);
    extraButtonsLayout->addStretch();
    extraButtonsLayout->addWidget(shuffleButton);
    extraButtonsLayout->addWidget(clearButton);
    extraButtonsLayout->addWidget(repeatButton);
    extraButtonsLayout->addStretch();

    centerLayout->addWidget(albumArtLabel);
    centerLayout->addWidget(visualizerWidget);
    centerLayout->addLayout(timeLayout);
    centerLayout->addLayout(extraButtonsLayout);

    // Right side: Stop, Next
    QVBoxLayout *rightLayout = new QVBoxLayout();
    stopButton = new QPushButton("Stop", controls);
    nextButton = new QPushButton("Next", controls);
    stopButton->setFixedSize(buttonSize);
    nextButton->setFixedSize(buttonSize);
    rightLayout->addWidget(stopButton);
    rightLayout->addSpacing(10); // Reduced spacing
    rightLayout->addWidget(nextButton);

    // Combine layouts with stretch for centering album art
    controlLayout->addLayout(leftLayout);
    controlLayout->addStretch(); // Center album art
    controlLayout->addLayout(centerLayout);
    controlLayout->addStretch(); // Center album art
    controlLayout->addLayout(rightLayout);
    mainLayout->addWidget(controls);

    // Gap between controls and playlist
    mainLayout->addSpacing(20);

    // Playlist
    playlistWidget = new QListWidget(central);
    mainLayout->addWidget(playlistWidget, 1); // Stretch factor for taller playlist

    setCentralWidget(central);

    // Connect buttons and slider
    connect(playPauseButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::playPause);
    connect(stopButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::stop);
    connect(nextButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::next);
    connect(prevButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::prev);
    connect(seekSlider, &QSlider::sliderMoved, this, &QxMusicPlayerWindow::seekPosition);
    connect(playlistWidget, &QListWidget::itemSelectionChanged, this, &QxMusicPlayerWindow::playlistSelectionChanged);
    connect(playlistWidget, &QListWidget::itemDoubleClicked, this, &QxMusicPlayerWindow::playSelectedTrack);
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

QString QxMusicPlayerWindow::getDisplayName(const QString &filePath, const QString &baseDir) {
    QString path = baseDir.isEmpty() ? filePath : filePath.mid(baseDir.length() + 1);
    QStringList segments = path.split('/', QString::SkipEmptyParts);
    QString artist, album, song;
    if (segments.size() >= 3) {
        artist = segments[segments.size() - 3];
        album = segments[segments.size() - 2];
        song = segments[segments.size() - 1];
        return QString("%1 - %2 - %3").arg(artist, album, song);
    } else if (segments.size() == 2) {
        album = segments[0];
        song = segments[1];
        return QString("%1 - %2").arg(album, song);
    }
    return baseDir.isEmpty() ? QFileInfo(filePath).fileName() : path;
}

void QxMusicPlayerWindow::openFile() {
    QTime timer;
    timer.start();
    QStringList files = QFileDialog::getOpenFileNames(this, "Open Audio Files", QDir::homePath(),
                                                     "Audio Files (*.mp3 *.flac *.wav)");
    QString baseDir; // Empty for single file selection
    for (const QString &file : files) {
        playlist->addMedia(QUrl::fromLocalFile(file));
        playlistWidget->addItem(getDisplayName(file, baseDir));
    }
    qDebug() << "openFile took" << timer.elapsed() << "ms";
}

void QxMusicPlayerWindow::openUrl() {
    QString url = urlBar->text();
    if (!url.isEmpty()) {
        playlist->addMedia(QUrl(url));
        playlistWidget->addItem(url);
    }
}

void QxMusicPlayerWindow::openDirectory() {
    QTime timer;
    timer.start();
    QString dir = QFileDialog::getExistingDirectory(this, "Open Directory", QDir::homePath());
    int fileCount = 0;
    if (!dir.isEmpty()) {
        QDirIterator it(dir, {"*.mp3", "*.flac", "*.wav"}, QDir::Files, QDirIterator::Subdirectories);
        int depth = 0;
        while (it.hasNext() && depth <= 1) {
            QString file = it.next();
            playlist->addMedia(QUrl::fromLocalFile(file));
            playlistWidget->addItem(getDisplayName(file, dir));
            fileCount++;
            if (it.fileInfo().absolutePath().count('/') > dir.count('/') + 1) {
                depth = it.fileInfo().absolutePath().count('/') - dir.count('/');
            }
        }
    }
    qDebug() << "openDirectory took" << timer.elapsed() << "ms, processed" << fileCount << "files";
}

void QxMusicPlayerWindow::handleMediaError() {
    qDebug() << "Media error:" << player->errorString();
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

void QxMusicPlayerWindow::toggleVisualizer() {
    showingVisualizer = !showingVisualizer;
    if (showingVisualizer) {
        albumArtLabel->hide();
        visualizerWidget->show();
    } else {
        visualizerWidget->hide();
        albumArtLabel->show();
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

void QxMusicPlayerWindow::updateAlbumArt() {
    QTime timer;
    timer.start();
    QMediaContent media = player->currentMedia();
    QString filePath = media.request().url().toLocalFile();
    if (filePath.isEmpty()) {
        albumArtLabel->setText("No Art");
        albumArtLabel->setPixmap(QPixmap());
        qDebug() << "updateAlbumArt took" << timer.elapsed() << "ms, no file path";
        return;
    }

    // Check for front.jpg or Front.jpg in the file's folder
    QDir dir = QFileInfo(filePath).absoluteDir();
    QStringList nameFilters = {"*.jpg", "*.JPG"}; // Case-insensitive
    QStringList images = dir.entryList(nameFilters, QDir::Files);
    QString imagePath;
    for (const QString &image : images) {
        if (image.toLower() == "front.jpg") {
            imagePath = dir.filePath(image);
            break;
        }
    }
    if (!imagePath.isEmpty()) {
        QImage image(imagePath);
        if (!image.isNull()) {
            albumArtLabel->setPixmap(QPixmap::fromImage(image).scaled(100, 100, Qt::KeepAspectRatio));
            qDebug() << "updateAlbumArt took" << timer.elapsed() << "ms, loaded" << imagePath;
            return;
        } else {
            qDebug() << "updateAlbumArt took" << timer.elapsed() << "ms, failed to load" << imagePath;
        }
    } else {
        qDebug() << "updateAlbumArt took" << timer.elapsed() << "ms, no front.jpg found in" << dir.path();
    }

    // No art found
    albumArtLabel->setText("No Art");
    albumArtLabel->setPixmap(QPixmap());
}
