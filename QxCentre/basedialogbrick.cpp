#include "basedialogbrick.h"
#include <QDebug>
#include <QSettings>
#include <QScreen>
#include <QGuiApplication>
#include <QCloseEvent>
#include "themebrick.h"

BaseDialogBrick::BaseDialogBrick(const QString &title, QWidget *parent)
    : QDialog(parent), title(title), themeBrick(nullptr) {
    setWindowTitle(title);
    QSettings settings("QxSeries", "QxCentre");
    QString theme = settings.value("theme", "dark").toString();
    initializeTheme(theme == "dark");
}

BaseDialogBrick::~BaseDialogBrick() {
    if (themeBrick) {
        delete themeBrick;
        themeBrick = nullptr;
    }
    qDebug() << "BaseDialogBrick destroyed for" << title;
}

void BaseDialogBrick::initializeTheme(bool isDark) {
    themeBrick = new ThemeBrick(qApp, this);
    themeBrick->toggleDarkTheme(isDark);
    qDebug() << "Initialized theme for" << title << ":" << (isDark ? "dark" : "light");
}

void BaseDialogBrick::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int x = (screenGeometry.width() - width()) / 2 + screenGeometry.left();
    int y = (screenGeometry.height() - height()) / 2 + screenGeometry.top();
    move(x, y);
    qDebug() << "Centered" << title << "at:" << x << y << "on screen:" << screen->name();
}

void BaseDialogBrick::saveGeometry() {
    QSettings settings("QxSeries", "QxCentre");
    settings.setValue(QString("window_position/%1").arg(title), geometry());
    qDebug() << "Saved geometry for" << title << ":" << geometry();
}

void BaseDialogBrick::closeEvent(QCloseEvent *event) {
    saveGeometry();
    QDialog::closeEvent(event);
}
