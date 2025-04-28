#ifndef INTERLINKBRICK_H
#define INTERLINKBRICK_H

#include <QObject>
#include <QMap>
#include <QPointer>
#include <QWidget>

class MainWindowBrick;

class InterlinkBrick : public QObject {
    Q_OBJECT
public:
    explicit InterlinkBrick(MainWindowBrick *mainWindow, QObject *parent = nullptr);
    ~InterlinkBrick(); // Added
    void registerAppWindow(const QString &name, QWidget *window);
    void unregisterAppWindow(const QString &name);
    void clearAppWindows();
    void launchAppWindow(const QString &name);
    QMap<QString, QPointer<QWidget>> getAppWindows() const;
    void raiseGroup();

private:
    MainWindowBrick *mainWindow;
    QMap<QString, QPointer<QWidget>> appWindows;
};

#endif // INTERLINKBRICK_H
