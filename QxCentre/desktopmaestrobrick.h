#ifndef DESKTOPMAESTROBRICK_H
#define DESKTOPMAESTROBRICK_H

#include <QObject>
#include <QMainWindow>

class DesktopMaestroBrick : public QObject {
    Q_OBJECT
public:
    static DesktopMaestroBrick* instance();
    void initializeDesktop();
    bool isDesktopVisible() const;

private:
    DesktopMaestroBrick(QObject *parent = nullptr);
    QMainWindow *mainWindow = nullptr;
};

#endif
