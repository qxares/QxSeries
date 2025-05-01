#ifndef STARTUPBRICK_H
#define STARTUPBRICK_H

#include <QObject>

class StartupBrick : public QObject {
    Q_OBJECT
public:
    explicit StartupBrick(QObject *parent = nullptr);
    ~StartupBrick();
    bool initialize();
    void shutdown();

private:
    bool initSettings();
};

#endif // STARTUPBRICK_H
