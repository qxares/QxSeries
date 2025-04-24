#ifndef WINDOWMANAGERBRICK_H
#define WINDOWMANAGERBRICK_H

#include <QObject>
#include <QMainWindow>
#include <QWidget>
#include <QList>

class WindowManagerBrick : public QObject {
    Q_OBJECT
public:
    explicit WindowManagerBrick(QMainWindow *mainWindow, QObject *parent = nullptr);
    void registerAppWindow(QWidget *appWindow);
    void launchAppWindow(QWidget *appWindow);

private slots:
    void handleMainWindowActivated();

private:
    void raiseAllWindows();
    bool eventFilter(QObject *obj, QEvent *event) override;
    QMainWindow *mainWindow;
    QList<QWidget*> appWindows;
};

#endif // WINDOWMANAGERBRICK_H
