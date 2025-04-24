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
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tbytevector.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
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
    connect(playlist, &QMediaPlaylist::currentIndexChanged, this, &QxMusicPlayerWindow::updateAlbumArt);
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
    controlLayout->setContentsMargins(15, 0, 15, 0); // 15pt left/right margins

    // Left side: Play/Pause, Prev
    QVBoxLayout *leftLayout = new QVBoxLayout();
    playPauseButton = new QPushButton("Play/Pause", controls);
    prevButton = new QPushButton("Prev", controls);
    QSize buttonSize(100, 30);
    playPauseButton->setFixedSize(buttonSize);
    prevButton->setFixedSize(buttonSize);
    leftLayout->addWidget(playPauseButton);
    leftLayout->addSpacing(20); // 20pt vertical spacing
    leftLayout->addWidget(prevButton);
    leftLayout->addStretch(); // Push buttons up

    // Center: Album art/visualization + time bar
    QVBoxLayout *centerLayout = new QVBoxLayout();
    albumArtLabel = new QLabel(controls);
    albumArtLabel->setFixedSize(100, 100);
    albumArtLabel->setAlignment(Qt::AlignCenter);
    albumArtLabel->setStyleSheet("background-color: gray; color: white;"); // Placeholder
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

    centerLayout->addWidget(albumArtLabel);
    centerLayout->addWidget(visualizerWidget);
    centerLayout->addLayout(timeLayout);
    centerLayout->addStretch(); // Push content up

    // Right side: Stop, Next
    QVBoxLayout *rightLayout = new QVBoxLayout();
    stopButton = new QPushButton("Stop", controls);
    nextButton = new QPushButton("Next", controls);
    stopButton->setFixedSize(buttonSize);
    nextButton->setFixedSize(buttonSize);
    rightLayout->addWidget(stopButton);
    rightLayout->addSpacing(20); // 20pt vertical spacing
    rightLayout->addWidget(nextButton);
    rightLayout->addStretch(); // Push buttons up

    // Combine layouts
    controlLayout->addLayout(leftLayout);
    controlLayout->addLayout(centerLayout);
    controlLayout->addLayout(rightLayout);
    mainLayout->addWidget(controls);

    // Playlist
    playlistWidget = new QListWidget(central);
    mainLayout->addWidget(playlistWidget);

    setCentralWidget(central);

    // Connect buttons and slider
    connect(playPauseButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::playPause);
    connect(stopButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::stop);
    connect(nextButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::next);
    connect(prevButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::prev);
    connect(seekSlider, &QSlider::sliderMoved, this, &QxMusicPlayerWindow::seekPosition);
    connect(playlistWidget, &QListWidget::itemSelectionChanged, this, &QxMusicPlayerWindow::playlistSelectionChanged);
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
    TagLib::FileRef file(filePath.toStdString().c_str());
    if (!file.isNull() && file.tag()) {
        QString artist = QString::fromStdString(file.tag()->artist().to8Bit(true));
        QString album = QString::fromStdString(file.tag()->album().to8Bit(true));
        QString title = QString::fromStdString(file.tag()->title().to8Bit(true));
        if (!artist.isEmpty() && !album.isEmpty() && !title.isEmpty()) {
            return QString("%1 - %2 - %3").arg(artist, album, title);
        }
    }
    // Fallback to directory name and file name
    QString dirName = baseDir.isEmpty() ? QFileInfo(filePath).absolutePath().section('/', -1) : baseDir.section('/', -1);
    QString fileName = QFileInfo(filePath).fileName();
    return QString("%1/%2").arg(dirName, fileName);
}

void QxMusicPlayerWindow::openFile() {
    QStringList files = QFileDialog::getOpenFileNames(this, "Open Audio Files", QDir::homePath(),
                                                     "Audio Files (*.mp3 *.flac *.wav)");
    QString baseDir; // Empty for single file selection
    for (const QString &file : files) {
        playlist->addMedia(QUrl::fromLocalFile(file));
        playlistWidget->addItem(getDisplayName(file, baseDir));
    }
}

void QxMusicPlayerWindow::openUrl() {
    QString url = urlBar->text();
    if (!url.isEmpty()) {
        playlist->addMedia(QUrl(url));
        playlistWidget->addItem(url);
    }
}

void QxMusicPlayerWindow::openDirectory() {
    QString dir = QFileDialog::getExistingDirectory(this, "Open Directory", QDir::homePath());
    if (!dir.isEmpty()) {
        QDirIterator it(dir, {"*.mp3", "*.flac", "*.wav"}, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString file = it.next();
            playlist->addMedia(QUrl::fromLocalFile(file));
            playlistWidget->addItem(getDisplayName(file, dir));
        }
    }
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
    QMediaContent media = player->currentMedia();
    QString filePath = media.request().url().toLocalFile();
    if (filePath.isEmpty()) {
        albumArtLabel->setText("No Art");
        albumArtLabel->setPixmap(QPixmap());
        return;
    }

    // Check ID3 tags for embedded album art
    TagLib::FileRef file(filePath.toStdString().c_str());
    if (!file.isNull() && file.tag()) {
        TagLib::MPEG::File mpegFile(filePath.toStdString().c_str());
        if (mpegFile.ID3v2Tag()) {
            TagLib::ID3v2::Tag *tag = mpegFile.ID3v2Tag();
            TagLib::ID3v2::FrameList frames = tag->frameList("APIC");
            if (!frames.isEmpty()) {
                TagLib::ID3v2::AttachedPictureFrame *picFrame =
                    static_cast<TagLib::ID3v2::AttachedPictureFrame*>(frames.front());
                QImage image;
                image.loadFromData((const uchar*)picFrame->picture().data(), picFrame->picture().size());
                if (!image.isNull()) {
                    albumArtLabel->setPixmap(QPixmap::fromImage(image).scaled(100, 100, Qt::KeepAspectRatio));
                    return;
                }
            }
        }
    }

    // Fallback to front.jpg or Front.jpg in the file's folder
    QDir dir = QFileInfo(filePath).absoluteDir();
    QStringList nameFilters = {"front.jpg", "Front.jpg"};
    QStringList images = dir.entryList(nameFilters, QDir::Files);
    if (!images.isEmpty()) {
        QImage image(dir.filePath(images.first()));
        if (!image.isNull()) {
            albumArtLabel->setPixmap(QPixmap::fromImage(image).scaled(100, 100, Qt::KeepAspectRatio));
            return;
        }
    }

    // No art found
    albumArtLabel->setText("No Art");
    albumArtLabel->setPixmap(QPixmap());
}
