#ifndef THEMEBRICK_H
#define THEMEBRICK_H

#include <QObject>
#include <QApplication>

class ThemeBrick : public QObject {
    Q_OBJECT
public:
    ThemeBrick(QApplication *app, QObject *parent = nullptr);

public slots:
    void toggleDarkTheme(bool checked);

signals:
    void themeChanged(bool dark);

private:
    void applyDarkTheme();
    QApplication *application;
};

#endif // THEMEBRICK_H
