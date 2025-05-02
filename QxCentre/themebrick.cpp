#include "themebrick.h"
#include <QDebug>
#include <QStyleFactory>
#include <QSettings>

ThemeBrick::ThemeBrick(QApplication *app, QObject *parent)
    : QObject(parent), application(app), darkThemeEnabled(true) {
    qDebug() << "ThemeBrick initialized";
    QSettings settings("QxSeries", "QxCentre");
    QString theme = settings.value("theme", "dark").toString();
    darkThemeEnabled = (theme.toLower() == "dark");
    if (darkThemeEnabled) {
        applyDarkTheme();
    } else {
        applyLightTheme();
    }
}

ThemeBrick::~ThemeBrick() {
    qDebug() << "ThemeBrick destroyed";
}

void ThemeBrick::setTheme(const QString &theme) {
    bool enableDark = (theme.toLower() == "dark");
    toggleDarkTheme(enableDark);
    QSettings settings("QxSeries", "QxCentre");
    settings.setValue("theme", enableDark ? "dark" : "light");
    qDebug() << "Saved theme to QSettings:" << (enableDark ? "dark" : "light");
}

void ThemeBrick::toggleDarkTheme(bool enable) {
    if (enable != darkThemeEnabled) {
        darkThemeEnabled = enable;
        if (enable) {
            applyDarkTheme();
        } else {
            applyLightTheme();
        }
        emit themeChanged(darkThemeEnabled);
        qDebug() << "Theme switched to" << (darkThemeEnabled ? "dark" : "light");
    }
}

bool ThemeBrick::isDarkTheme() const {
    return darkThemeEnabled;
}

void ThemeBrick::applyDarkTheme() {
    QPalette monokaiPalette;
    monokaiPalette.setColor(QPalette::Window, QColor("#272822")); // Monokai background
    monokaiPalette.setColor(QPalette::WindowText, QColor("#F8F8F2")); // Monokai foreground
    monokaiPalette.setColor(QPalette::Base, QColor("#272822")); // Editor background
    monokaiPalette.setColor(QPalette::AlternateBase, QColor("#3E3D32")); // Slightly lighter for contrast
    monokaiPalette.setColor(QPalette::ToolTipBase, QColor("#75715E")); // Monokai comment
    monokaiPalette.setColor(QPalette::ToolTipText, QColor("#F8F8F2")); // Foreground
    monokaiPalette.setColor(QPalette::Text, QColor("#F8F8F2")); // Text
    monokaiPalette.setColor(QPalette::Button, QColor("#3E3D32")); // Button background
    monokaiPalette.setColor(QPalette::ButtonText, QColor("#F8F8F2")); // Button text
    monokaiPalette.setColor(QPalette::BrightText, QColor("#F8F8F2")); // Keywords (off-white)
    monokaiPalette.setColor(QPalette::Link, QColor("#d6d6c7")); // Links (light gray)
    monokaiPalette.setColor(QPalette::Highlight, QColor("#3e3d33")); // Selection (dark gray)
    monokaiPalette.setColor(QPalette::HighlightedText, QColor("#F8F8F2")); // Foreground for contrast
    application->setPalette(monokaiPalette);
    application->setStyle(QStyleFactory::create("Fusion"));
}

void ThemeBrick::applyLightTheme() {
    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, Qt::white);
    lightPalette.setColor(QPalette::WindowText, Qt::black);
    lightPalette.setColor(QPalette::Base, Qt::white);
    lightPalette.setColor(QPalette::AlternateBase, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::ToolTipBase, Qt::black);
    lightPalette.setColor(QPalette::ToolTipText, Qt::black);
    lightPalette.setColor(QPalette::Text, Qt::black);
    lightPalette.setColor(QPalette::Button, Qt::white);
    lightPalette.setColor(QPalette::ButtonText, Qt::black);
    lightPalette.setColor(QPalette::BrightText, Qt::red);
    lightPalette.setColor(QPalette::Link, QColor(0, 0, 255));
    lightPalette.setColor(QPalette::Highlight, QColor(0, 120, 215));
    lightPalette.setColor(QPalette::HighlightedText, Qt::white);
    application->setPalette(lightPalette);
    application->setStyle(QStyleFactory::create("Fusion"));
}

void ThemeBrick::disconnectThemeSignals(QWidget *widget) {
    if (widget) {
        disconnect(this, &ThemeBrick::themeChanged, widget, nullptr);
        qDebug() << "Disconnected theme signals for widget";
    }
}
