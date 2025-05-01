#ifndef WINDOWBRICK_H
#define WINDOWBRICK_H

#include <QMainWindow>
#include "themebrick.h"
#include "interlinkbrick.h"

class WindowBrick : public QMainWindow {
    Q_OBJECT
public:
    explicit WindowBrick(const QString &appName, QWidget *parent = nullptr);
    virtual ~WindowBrick();
    void initializeTheme(bool isDark);
    virtual void postInitialize();
    void centerWindow();

signals:
    void windowClosed(QObject *obj);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QString appName;
    ThemeBrick *themeBrick;
    InterlinkBrick *interlinkBrick;
};

#endif // WINDOWBRICK_H
