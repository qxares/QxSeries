#include "windowbrick.h"
#include <QDebug>
#include <QCloseEvent>
#include <QScreen>
#include <QSettings>

WindowBrick::WindowBrick(const QString &appName, QWidget *parent)
    : QMainWindow(parent), appName(appName) {
    setWindowTitle(appName);
    setMinimumSize(400, 300);
    themeBrick = nullptr;
    interlinkBrick = nullptr;
    qDebug() << "WindowBrick initialized for" << appName;
}

WindowBrick::~WindowBrick() {
    qDebug() << "WindowBrick destroyed for" << appName;
}

void WindowBrick::initializeTheme(bool isDark) {
    if (!themeBrick) {
        themeBrick = new ThemeBrick(qApp, this);
    }
    themeBrick->toggleDarkTheme(isDark);
    qDebug() << "Initialized theme for" << appName << ":" << (isDark ? "dark" : "light");
}

void WindowBrick::postInitialize() {
    qDebug() << "Post-initialization for" << appName;
}

void WindowBrick::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    QPoint cursorPos = QCursor::pos();
    for (QScreen *s : QGuiApplication::screens()) {
        if (s->geometry().contains(cursorPos)) {
            screen = s;
            screenGeometry = s->availableGeometry();
            break;
        }
    }
    int x = (screenGeometry.width() - width()) / 2 + screenGeometry.left();
    int y = (screenGeometry.height() - height()) / 2 + screenGeometry.top();
    move(x, y);
    QSettings settings("QxSeries", "QxCentre");
    settings.setValue("window_position/" + appName, geometry());
    qDebug() << "Centered" << appName << "at:" << x << y << "on screen:" << screen->name();
}

void WindowBrick::closeEvent(QCloseEvent *event) {
    QSettings settings("QxSeries", "QxCentre");
    settings.setValue("window_position/" + appName, geometry());
    emit windowClosed(this);
    event->accept();
    qDebug() << "Close event for" << appName;
}
