#ifndef TEST_THEME_H
#define TEST_THEME_H

#include <QObject>
#include <QApplication>
#include <QPalette>

class ThemeBrick : public QObject {
    Q_OBJECT
public:
    ThemeBrick(QApplication *app);
    void setTheme(const QString &theme);
    bool isDarkTheme() const { return darkThemeEnabled; }

private:
    void applyDarkTheme();
    void applyLightTheme();

    QApplication *application;
    bool darkThemeEnabled;
};

#endif // TEST_THEME_H
