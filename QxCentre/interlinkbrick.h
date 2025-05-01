#ifndef INTERLINKBRICK_H
#define INTERLINKBRICK_H

#include <QObject>
#include <QMap>
#include <QPointer>
#include <QString>
#include "mainwindowbrick.h"

class InterlinkBrick : public QObject {
    Q_OBJECT

public:
    explicit InterlinkBrick(MainWindowBrick *parent);
    ~InterlinkBrick();

    void registerAppWindow(const QString &appName, QWidget *window);
    void unregisterAppWindow(const QString &appName);
    void restoreWindow(const QString &appName);
    void launchAppWindow(const QString &appName);
    QMap<QString, QPointer<QWidget>> getAppWindows() const;

signals:
    void windowStateChanged();

private:
    QMap<QString, QPointer<QWidget>> appWindows;
    MainWindowBrick *mainWindow;
};

#endif // INTERLINKBRICK_H
