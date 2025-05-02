#ifndef WINDOWBRICK_H
#define WINDOWBRICK_H

#include <QMainWindow>
#include "themebrick.h"

class InterlinkBrick; // Forward declaration

class WindowBrick : public QMainWindow {
    Q_OBJECT
public:
    explicit WindowBrick(const QString &appName, QWidget *parent = nullptr);
    virtual ~WindowBrick();
    void initializeTheme(bool isDark);
    virtual void postInitialize();
    void centerWindow();

signals:
    void windowClosed(WindowBrick *window);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QString appName;
    ThemeBrick *themeBrick;
    InterlinkBrick *interlinkBrick;
};

#endif // WINDOWBRICK_H
