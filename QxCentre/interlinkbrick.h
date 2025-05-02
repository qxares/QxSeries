#ifndef INTERLINKBRICK_H
#define INTERLINKBRICK_H

#include <QObject>
#include <QList>
#include "windowbrick.h"

class InterlinkBrick : public QObject {
    Q_OBJECT
public:
    explicit InterlinkBrick(QObject *parent = nullptr);
    ~InterlinkBrick();

    void incrementOpenAppCount();
    void decrementOpenAppCount();
    void activateWindow(int index);
    void updateTaskbarWindows();
    void addWindow(WindowBrick *window);
    int getOpenAppCount() const;

signals:
    void taskbarUpdated(const QStringList &windowTitles);
    void appCountChanged(int count);

private slots:
    void handleWindowClosed(WindowBrick *window);

private:
    QList<WindowBrick*> openWindows;
    int openAppCount;
};

#endif // INTERLINKBRICK_H
