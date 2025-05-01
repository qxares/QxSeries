#include "themebrick.h"
#include <QStyle>
#include <QPalette>
#include <QDebug>

ThemeBrick::ThemeBrick(QApplication *app, QObject *parent) : QObject(parent), application(app) {
    isDark = true;
    applyDarkTheme();
    qDebug() << "ThemeBrick initialized with dark theme";
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
    applyGlobalStyles();
    emit themeChanged(true);
}

void ThemeBrick::applyGlobalStyles() {
    QString styleSheet = 
        "QWidget, QMdiSubWindow {"
        "    background-color: %1;"
        "    color: %2;"
        "    font-family: 'Arial', sans-serif;"
        "    font-size: 12px;"
        "}"
        "QToolBar, QDockWidget {"
        "    background-color: %3;"
        "    color: %2;"
        "    border: 1px solid %4;"
        "}"
        "QPushButton, QComboBox {"
        "    background-color: %3;"
        "    color: %2;"
        "    border: 1px solid %4;"
        "    padding: 4px;"
        "}"
        "QPushButton:hover, QComboBox:hover {"
        "    background-color: %5;"
        "}"
        "QMenu {"
        "    background-color: %3;"
        "    color: %2;"
        "}"
        "QMenu::item:selected {"
        "    background-color: %5;"
        "}";
    if (isDark) {
        styleSheet = styleSheet.arg("#272822", "#F8F8F2", "#49483E", "#3E3D32", "#0D3B44");
    } else {
        styleSheet = styleSheet.arg("#ECECEC", "#000000", "#D3D3D3", "#B0B0B0", "#0078D7");
    }
    application->setStyleSheet(styleSheet);
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
        applyGlobalStyles();
        emit themeChanged(false);
    }
    application->setStyle("Fusion");
}

void ThemeBrick::loadThemeSettings() {
    // Placeholder for DatabaseBrick integration
    qDebug() << "Loading theme settings (placeholder)";
}

void ThemeBrick::disconnectThemeSignals(QObject *receiver) {
    if (receiver) {
        disconnect(this, &ThemeBrick::themeChanged, receiver, nullptr);
        qDebug() << "Disconnected themeChanged signals for receiver";
    }
}
