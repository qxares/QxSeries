#ifndef INTERLINKMAESTROBRICK_H
#define INTERLINKMAESTROBRICK_H

#include <QObject>
#include <QMap>
#include <QStringList>
#include <QPointer>
#include <QWidget>

class InterlinkMaestroBrick : public QObject {
    Q_OBJECT
public:
    explicit InterlinkMaestroBrick(QObject *parent = nullptr);
    ~InterlinkMaestroBrick();
    void registerApp(const QString &appName, QWidget *window, const QStringList &dependencies);
    void unregisterApp(const QString &appName);
    void updateApps(const QString &updateType);

signals:
    void appRegistered(const QString &appName);
    void appUnregistered(const QString &appName);
    void appUpdated(const QString &appName, const QString &updateType);

private:
    struct AppRoadmap {
        QPointer<QWidget> window;
        QStringList dependencies;
    };
    QMap<QString, AppRoadmap> appRoadmaps;
};

#endif // INTERLINKMAESTROBRICK_H
