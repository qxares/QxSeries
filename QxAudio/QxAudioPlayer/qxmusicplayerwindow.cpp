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
    setupMenus();
    setupCentralWidget();
    qDebug() << "QxMusicPlayer main window initialized";
}

void QxMusicPlayerWindow::initializeTheme(bool isDark) {
    themeBrick->toggleDarkTheme(isDark);
    darkThemeAction->setChecked(isDark);
    // Apply theme to URL bar
    QPalette palette = qApp->palette();
    urlBar->setPalette(palette);
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

    controlLayout->addLayout(leftLayout);
    controlLayout->addStretch(); // Center layouts
    controlLayout->addLayout(rightLayout);
    controls->setLayout(controlLayout);

    // Playlist
    playlistWidget = new QListWidget(central);
    connect(playlistWidget, &QListWidget::itemSelectionChanged, this, &QxMusicPlayerWindow::playlistSelectionChanged);

    mainLayout->addWidget(controls);
    mainLayout->addWidget(playlistWidget, 1); // Stretch to fill space
    central->setLayout(mainLayout);
    setCentralWidget(central);

    // Connect buttons
    connect(playPauseButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::playPause);
    connect(stopButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::stop);
    connect(nextButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::next);
    connect(prevButton, &QPushButton::clicked, this, &QxMusicPlayerWindow::prev);
}

void QxMusicPlayerWindow::toggleDarkTheme() {
    themeBrick->toggleDarkTheme(darkThemeAction->isChecked());
    // Update URL bar palette
    QPalette palette = qApp->palette();
    urlBar->setPalette(palette);
}

void QxMusicPlayerWindow::handleWindowStateChange(bool minimized) {
    if (minimized) {
        showMinimized();
        qDebug() << "QxMusicPlayer minimized via InterlinkBrick";
    } else {
        if (windowState() & Qt::WindowMaximized) {
            showMaximized();
        } else {
            showNormal();
        }
        raise(); // Ensure QxMusicPlayer stays above QxCentre
        qDebug() << "QxMusicPlayer restored via InterlinkBrick";
    }
}

void QxMusicPlayerWindow::playPause() {
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
        qDebug() << "Playback paused";
    } else if (player->state() == QMediaPlayer::PausedState || player->state() == QMediaPlayer::StoppedState) {
        player->play();
        qDebug() << "Playback started";
    }
}

void QxMusicPlayerWindow::stop() {
    player->stop();
    qDebug() << "Playback stopped";
}

void QxMusicPlayerWindow::next() {
    if (playlist->mediaCount() > 0) {
        playlist->next();
        qDebug() << "Next track";
    }
}

void QxMusicPlayerWindow::prev() {
    if (playlist->mediaCount() > 0) {
        playlist->previous();
        qDebug() << "Previous track";
    }
}

void QxMusicPlayerWindow::openFile() {
    QFileDialog dialog(this, "Open Audio Files");
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter("Audio Files (*.mp3 *.flac *.ogg *.wav)");
    dialog.setDirectory(QDir::homePath());
    // Apply theme (global DontUseNativeDialogs handles Qt-native)
    QPalette palette = qApp->palette();
    dialog.setPalette(palette);
    if (dialog.exec()) {
        QStringList files = dialog.selectedFiles();
        playlist->clear();
        playlistWidget->clear();
        for (const QString &file : files) {
            playlist->addMedia(QUrl::fromLocalFile(file));
            playlistWidget->addItem(QFileInfo(file).fileName());
            qDebug() << "Added file to playlist:" << file;
        }
        if (!files.isEmpty()) {
            playlist->setCurrentIndex(0);
            if (player->state() != QMediaPlayer::PlayingState) {
                player->play();
                qDebug() << "Started playback after adding files";
            }
        }
    }
}

void QxMusicPlayerWindow::openDirectory() {
    QFileDialog dialog(this, "Open Directory");
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory(QDir::homePath());
    // Apply theme
    QPalette palette = qApp->palette();
    dialog.setPalette(palette);
    if (dialog.exec()) {
        QString dirPath = dialog.selectedFiles().first();
        playlist->clear();
        playlistWidget->clear();
        QDirIterator it(dirPath, {"*.mp3", "*.flac", "*.ogg", "*.wav"}, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString file = it.next();
            playlist->addMedia(QUrl::fromLocalFile(file));
            playlistWidget->addItem(QFileInfo(file).fileName());
            qDebug() << "Added file to playlist:" << file;
        }
        if (playlist->mediaCount() > 0) {
            playlist->setCurrentIndex(0);
            if (player->state() != QMediaPlayer::PlayingState) {
                player->play();
                qDebug() << "Started playback after adding directory";
            }
        }
    }
}

void QxMusicPlayerWindow::openUrl() {
    QString urlText = urlBar->text().trimmed();
    if (!urlText.isEmpty()) {
        QUrl url(urlText);
        if (url.isValid() && (url.scheme() == "http" || url.scheme() == "https")) {
            playlist->clear();
            playlistWidget->clear();
            playlist->addMedia(url);
            playlistWidget->addItem(urlText);
            player->setMedia(url);
            player->play();
            qDebug() << "Streaming URL:" << urlText;
        } else {
            qDebug() << "Invalid URL:" << urlText;
        }
    }
}

void QxMusicPlayerWindow::handleMediaError() {
    QString errorString = player->errorString();
    qDebug() << "Media error:" << errorString;
}

void QxMusicPlayerWindow::playlistSelectionChanged() {
    QList<QListWidgetItem*> selected = playlistWidget->selectedItems();
    if (!selected.isEmpty()) {
        int index = playlistWidget->row(selected.first());
        playlist->setCurrentIndex(index);
        if (player->state() != QMediaPlayer::PlayingState) {
            player->play();
            qDebug() << "Started playback from playlist selection";
        }
    }
}
