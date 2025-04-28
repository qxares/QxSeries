#include "themebrick.h"
#include <QStyle>
#include <QPalette>
#include <QDebug>

ThemeBrick::ThemeBrick(QApplication *app, QObject *parent) : QObject(parent), application(app) {
    qDebug() << "ThemeBrick initialized";
}

ThemeBrick::~ThemeBrick() {
    disconnect(this, nullptr, nullptr, nullptr);
    qDebug() << "ThemeBrick destroyed";
}

void ThemeBrick::applyDarkTheme() {
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor("#272822"));
    darkPalette.setColor(QPalette::WindowText, QColor("#F8F8F2"));
    darkPalette.setColor(QPalette::Base, QColor("#272822"));
    darkPalette.setColor(QPalette::Text, QColor("#F8F8F2"));
    darkPalette.setColor(QPalette::Button, QColor("#49483E"));
    darkPalette.setColor(QPalette::ButtonText, QColor("#F8F8F2"));
    darkPalette.setColor(QPalette::AlternateBase, QColor("#3E3D32"));
    darkPalette.setColor(QPalette::Highlight, QColor("#0D3B44"));
    darkPalette.setColor(QPalette::HighlightedText, QColor("#F8F8F2"));
    application->setPalette(darkPalette);

    application->setStyleSheet(
        "QWidget, QMdiSubWindow {"
        "    background-color: #272822;"
        "    color: #F8F8F2;"
        "}"
        "QMenuBar, QMenu {"
        "    background-color: #49483E;"
        "    color: #F8F8F2;"
        "}"
        "QMenu::item:selected {"
        "    background-color: #0D3B44;"
        "}"
    );

    emit themeChanged(true);
}

void ThemeBrick::toggleDarkTheme(bool checked) {
    isDark = checked;
    qDebug() << "Toggling theme to " << (checked ? "dark" : "light");
    if (checked) {
        applyDarkTheme();
    } else {
        QPalette lightPalette;
        lightPalette.setColor(QPalette::Window, QColor("#ECECEC"));
        lightPalette.setColor(QPalette::WindowText, Qt::black);
        lightPalette.setColor(QPalette::Base, QColor("#ECECEC"));
        lightPalette.setColor(QPalette::Text, Qt::black);
        lightPalette.setColor(QPalette::Button, QColor("#D3D3D3"));
        lightPalette.setColor(QPalette::ButtonText, Qt::black);
        lightPalette.setColor(QPalette::AlternateBase, QColor("#D3D3D3"));
        lightPalette.setColor(QPalette::Highlight, QColor("#0078D7"));
        lightPalette.setColor(QPalette::HighlightedText, Qt::white);
        application->setPalette(lightPalette);
        application->setStyleSheet("");
        emit themeChanged(false);
    }
    application->setStyle("Fusion");
}

void ThemeBrick::disconnectThemeSignals(QObject *receiver) {
    if (receiver) {
        disconnect(this, &ThemeBrick::themeChanged, receiver, nullptr);
        qDebug() << "Disconnected themeChanged signals for receiver";
    }
}
