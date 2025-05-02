#ifndef THEMEBRICK_H
#define THEMEBRICK_H

#include <QObject>
#include <QApplication>
#include <QPalette>
#include <QWidget>

class ThemeBrick : public QObject {
    Q_OBJECT
public:
    explicit ThemeBrick(QApplication *app, QObject *parent = nullptr);
    ~ThemeBrick();
    void setTheme(const QString &theme);
    void toggleDarkTheme(bool enable);
    bool isDarkTheme() const;
    void disconnectThemeSignals(QWidget *widget);

signals:
    void themeChanged(bool isDark);

private:
    void applyDarkTheme();
    void applyLightTheme();

    QApplication *application;
    bool darkThemeEnabled;
};

#endif // THEMEBRICK_H
