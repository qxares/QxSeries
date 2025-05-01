#ifndef INTERLINKBRICK_H
#define INTERLINKBRICK_H

#include <QObject>
#include <QMap>
#include <QPointer>
#include <QWidget>

class InterlinkBrick : public QObject {
    Q_OBJECT
public:
    explicit InterlinkBrick(QObject *parent = nullptr);
    ~InterlinkBrick();
    void launchAppWindow(const QString &appName);
    void restoreWindow(const QString &windowName);
    void registerAppWindow(const QString &name, QWidget *window);
    void unregisterAppWindow(const QString &name);
    QMap<QString, QPointer<QWidget>> getAppWindows() const;

signals:
    void windowStateChanged();

private:
    QMap<QString, QPointer<QWidget>> appWindows;
};

#endif // INTERLINKBRICK_H
