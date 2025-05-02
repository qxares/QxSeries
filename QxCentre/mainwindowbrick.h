#ifndef MAINWINDOWBRICK_H
#define MAINWINDOWBRICK_H

#include <QMainWindow>
#include <QWidget>
#include "themebrick.h"
#include "interlinkbrick.h"

class MainWindowBrick : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindowBrick(QWidget *parent = nullptr);
    ~MainWindowBrick();

    ThemeBrick* getThemeBrick();
    InterlinkBrick* getInterlinkBrick();

public slots:
    void centerWindow();
    void updateCentralWidgetStyle(bool isDark);
    void handleExit();
    void launchInfoWindow();
    void raiseGroup();
    void activateWindow(int index);
    void updateTaskbarWindows();

protected:
    void closeEvent(QCloseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void moveEvent(QMoveEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void handleAppWindowDestroyed(QObject *obj);
    void incrementOpenAppCount();
    void decrementOpenAppCount();

private:
    void setupTaskbar();
    ThemeBrick *themeBrick;
    InterlinkBrick *interlinkBrick;
};

#endif // MAINWINDOWBRICK_H
