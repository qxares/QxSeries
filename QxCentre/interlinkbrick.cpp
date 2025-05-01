#include "interlinkbrick.h"
#include <QDebug>
#include <QProcess>

InterlinkBrick::InterlinkBrick(QObject *parent)
    : QObject(parent) {
    qDebug() << "InterlinkBrick initialized";
}

InterlinkBrick::~InterlinkBrick() {
    qDebug() << "InterlinkBrick destroyed";
}

void InterlinkBrick::launchAppWindow(const QString &appName) {
    qDebug() << "Launching app window:" << appName;
    QProcess *process = new QProcess(this);
    QString program = appName.toLower();
    process->start(program, QStringList());
    if (!process->waitForStarted(3000)) {
        qDebug() << "Failed to start app:" << appName << ", error:" << process->errorString();
        delete process;
        return;
    }
    registerAppWindow(appName, nullptr);
    qDebug() << "App window launched:" << appName;
    emit windowStateChanged();
}

void InterlinkBrick::restoreWindow(const QString &windowName) {
    if (appWindows.contains(windowName) && appWindows[windowName]) {
        QPointer<QWidget> window = appWindows[windowName];
        if (window->isMinimized()) {
            window->showNormal();
        }
        window->raise();
        window->activateWindow();
        qDebug() << "Restored window:" << windowName;
        emit windowStateChanged();
    }
}

void InterlinkBrick::registerAppWindow(const QString &name, QWidget *window) {
    appWindows[name] = window;
    if (window) {
        connect(window, &QObject::destroyed, this, [=]() {
            appWindows.remove(name);
            qDebug() << "Unregistered app window on destruction:" << name;
            emit windowStateChanged();
        });
    }
    qDebug() << "Registered app window:" << name;
    emit windowStateChanged();
}

void InterlinkBrick::unregisterAppWindow(const QString &name) {
    if (appWindows.contains(name)) {
        appWindows.remove(name);
        qDebug() << "Unregistered app window:" << name;
        emit windowStateChanged();
    }
}

QMap<QString, QPointer<QWidget>> InterlinkBrick::getAppWindows() const {
    return appWindows;
}
