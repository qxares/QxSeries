#ifndef INTERLINKBRICK_H
#define INTERLINKBRICK_H

#include <QObject>
#include <QMainWindow>
#include <QWidget>
#include <QList>
#include <QMap>
#include <QWindow>

class InterlinkBrick : public QObject {
    Q_OBJECT
public:
    explicit InterlinkBrick(QMainWindow *mainWindow, QObject *parent = nullptr);
    void registerAppWindow(QWidget *appWindow);
    void launchAppWindow(QWidget *appWindow);

signals:
    void windowStateChanged(bool minimized);

private slots:
    void handleMainWindowActivated();
    void onAppWindowDestroyed(QObject *obj);

private:
    void raiseAllWindows();
    void minimizeAllWindows();
    void restoreAllWindows();
    bool eventFilter(QObject *obj, QEvent *event) override;
    QMainWindow *mainWindow;
    QList<QWidget*> appWindows;
    QMap<QWidget*, Qt::WindowStates> appWindowStates; // Track previous states
};

#endif // INTERLINKBRICK_H
