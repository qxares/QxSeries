#include "themebrick.h"
#include <QStyle>

ThemeBrick::ThemeBrick(QApplication *app, QObject *parent) : QObject(parent), application(app) {
}

void ThemeBrick::applyDarkTheme() {
    application->setStyle("Fusion");

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor("#272822"));
    darkPalette.setColor(QPalette::WindowText, QColor("#F8F8F2"));
    darkPalette.setColor(QPalette::Base, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::black);
    darkPalette.setColor(QPalette::Button, QColor("#272822"));
    darkPalette.setColor(QPalette::ButtonText, QColor("#F8F8F2"));
    darkPalette.setColor(QPalette::AlternateBase, Qt::white);
    application->setPalette(darkPalette);

    application->setStyleSheet(
        "QMainWindow {"
        "    background-color: #272822;"
        "    color: #F8F8F2;"
        "}"
        "QMenuBar {"
        "    background-color: #49483E;"
        "    color: #F8F8F2;"
        "}"
        "QMenuBar::item {"
        "    background-color: #49483E;"
        "    color: #F8F8F2;"
        "}"
        "QMenuBar::item:selected {"
        "    background-color: #0D3B44;"
        "}"
        "QMenu {"
        "    background-color: #49483E;"
        "    color: #F8F8F2;"
        "}"
        "QMenu::item:selected {"
        "    background-color: #0D3B44;"
        "}"
        "QListWidget {"
        "    background-color: #272822;"
        "    color: #F8F8F2;"
        "    border: 1px solid #49483E;"
        "}"
        "QListWidget::item:selected {"
        "    background-color: #0D3B44;"
        "}"
        "QWidget#centralWidget {"
        "    background-color: #272822;"
        "    color: #F8F8F2;"
        "    border: 1px solid #49483E;"
        "}"
    );

    application->style()->polish(application);
    emit themeChanged(true);
}

void ThemeBrick::toggleDarkTheme(bool checked) {
    if (checked) {
        applyDarkTheme();
    } else {
        application->setStyle("Fusion");

        QPalette nativePalette;
        nativePalette.setColor(QPalette::Window, QColor("#ECECEC"));
        nativePalette.setColor(QPalette::WindowText, Qt::black);
        nativePalette.setColor(QPalette::Base, Qt::white);
        nativePalette.setColor(QPalette::Text, Qt::black);
        nativePalette.setColor(QPalette::Button, QColor("#ECECEC"));
        nativePalette.setColor(QPalette::ButtonText, Qt::black);
        nativePalette.setColor(QPalette::AlternateBase, Qt::white);
        application->setPalette(nativePalette);
        application->setStyleSheet("");

        application->style()->polish(application);
        emit themeChanged(false);
    }
}
