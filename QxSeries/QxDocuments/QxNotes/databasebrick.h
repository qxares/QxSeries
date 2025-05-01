#ifndef DATABASEBRICK_H
#define DATABASEBRICK_H

#include <QObject>
#include <QSqlDatabase>

class DatabaseBrick : public QObject {
    Q_OBJECT
public:
    explicit DatabaseBrick(QObject *parent = nullptr);
    ~DatabaseBrick();
    bool initialize();
    void saveTheme(const QString &theme);
    QString loadTheme() const;
    void saveWindowPosition(const QString &windowName, int x, int y, int width, int height);
    bool loadWindowPosition(const QString &windowName, int &x, int &y, int &width, int &height) const;

private:
    QSqlDatabase db;
    bool createTables();
};

#endif // DATABASEBRICK_H
