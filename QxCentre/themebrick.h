#ifndef THEMEBRICK_H
#define THEMEBRICK_H

#include <QObject>
#include <QApplication>

class ThemeBrick : public QObject {
    Q_OBJECT
public:
    ThemeBrick(QApplication *app, QObject *parent = nullptr);
    ~ThemeBrick(); // Added
    bool isDarkTheme() const { return isDark; }
    void disconnectThemeSignals(QObject *receiver);

public slots:
    void toggleDarkTheme(bool checked);

signals:
    void themeChanged(bool dark);

private:
    void applyDarkTheme();
    QApplication *application;
    bool isDark = false;
};

#endif // THEMEBRICK_H
