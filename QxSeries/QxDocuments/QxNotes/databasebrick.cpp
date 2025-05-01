#include "databasebrick.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>

DatabaseBrick::DatabaseBrick(QObject *parent) : QObject(parent) {
}

DatabaseBrick::~DatabaseBrick() {
    if (db.isOpen()) {
        db.close();
    }
    qDebug() << "DatabaseBrick destroyed";
}

bool DatabaseBrick::initialize() {
    db = QSqlDatabase::addDatabase("QSQLITE", "qxcentre_db");
    QString dbPath = QDir::homePath() + "/QxCentre/QxCentre/qxcentre.db";
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Failed to open database:" << db.lastError().text();
        return false;
    }

    if (!createTables()) {
        qDebug() << "Failed to create tables";
        db.close();
        return false;
    }

    qDebug() << "DatabaseBrick initialized";
    return true;
}

bool DatabaseBrick::createTables() {
    QSqlQuery query(db);
    bool success = true;

    // Settings table for theme
    success &= query.exec("CREATE TABLE IF NOT EXISTS settings ("
                          "key TEXT PRIMARY KEY, "
                          "value TEXT)");
    if (!success) {
        qDebug() << "Failed to create settings table:" << query.lastError().text();
        return false;
    }

    // Window positions table
    success &= query.exec("CREATE TABLE IF NOT EXISTS window_positions ("
                          "window_name TEXT PRIMARY KEY, "
                          "x INTEGER, "
                          "y INTEGER, "
                          "width INTEGER, "
                          "height INTEGER)");
    if (!success) {
        qDebug() << "Failed to create window_positions table:" << query.lastError().text();
        return false;
    }

    return true;
}

void DatabaseBrick::saveTheme(const QString &theme) {
    QSqlQuery query(db);
    query.prepare("INSERT OR REPLACE INTO settings (key, value) VALUES (:key, :value)");
    query.bindValue(":key", "theme");
    query.bindValue(":value", theme);
    if (!query.exec()) {
        qDebug() << "Failed to save theme:" << query.lastError().text();
    } else {
        qDebug() << "Saved theme:" << theme;
    }
}

QString DatabaseBrick::loadTheme() const {
    QSqlQuery query(db);
    query.prepare("SELECT value FROM settings WHERE key = :key");
    query.bindValue(":key", "theme");
    if (query.exec() && query.next()) {
        QString theme = query.value(0).toString();
        qDebug() << "Loaded theme:" << theme;
        return theme;
    }
    qDebug() << "No theme found, returning default: dark";
    return "dark";
}

void DatabaseBrick::saveWindowPosition(const QString &windowName, int x, int y, int width, int height) {
    QSqlQuery query(db);
    query.prepare("INSERT OR REPLACE INTO window_positions (window_name, x, y, width, height) "
                  "VALUES (:window_name, :x, :y, :width, :height)");
    query.bindValue(":window_name", windowName);
    query.bindValue(":x", x);
    query.bindValue(":y", y);
    query.bindValue(":width", width);
    query.bindValue(":height", height);
    if (!query.exec()) {
        qDebug() << "Failed to save window position for" << windowName << ":" << query.lastError().text();
    } else {
        qDebug() << "Saved window position for" << windowName << ": x=" << x << ", y=" << y
                 << ", width=" << width << ", height=" << height;
    }
}

bool DatabaseBrick::loadWindowPosition(const QString &windowName, int &x, int &y, int &width, int &height) const {
    QSqlQuery query(db);
    query.prepare("SELECT x, y, width, height FROM window_positions WHERE window_name = :window_name");
    query.bindValue(":window_name", windowName);
    if (query.exec() && query.next()) {
        x = query.value(0).toInt();
        y = query.value(1).toInt();
        width = query.value(2).toInt();
        height = query.value(3).toInt();
        qDebug() << "Loaded window position for" << windowName << ": x=" << x << ", y=" << y
                 << ", width=" << width << ", height=" << height;
        return true;
    }
    qDebug() << "No window position found for" << windowName;
    return false;
}
