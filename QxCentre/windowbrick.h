#ifndef WINDOWBRICK_H
#define WINDOWBRICK_H

#include <QMainWindow>
#include "themebrick.h"
#include "interlinkbrick.h"

class MainWindowBrick;

class WindowBrick : public QMainWindow {
    Q_OBJECT
public:
    explicit WindowBrick(MainWindowBrick *mainWindow, QWidget *parent = nullptr);
    virtual ~WindowBrick();
    virtual void initializeTheme(bool isDark) = 0;
    virtual void postInitialize();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    MainWindowBrick *mainWindowBrick;
    ThemeBrick *themeBrick;
    InterlinkBrick *interlinkBrick;
};

#endif // WINDOWBRICK_H
