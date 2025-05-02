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
        qDebug() << "Emitted themeChanged, dark:" << darkThemeEnabled;
    }
}

bool ThemeBrick::isDarkTheme() const {
    return darkThemeEnabled;
}

void ThemeBrick::applyDarkTheme() {
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor("#2E2E2E")); // Flat-Color-GTK dark
    darkPalette.setColor(QPalette::WindowText, QColor("#FFFFFF"));
    darkPalette.setColor(QPalette::Base, QColor("#2E2E2E"));
    darkPalette.setColor(QPalette::AlternateBase, QColor("#3E3D32"));
    darkPalette.setColor(QPalette::ToolTipBase, QColor("#75715E"));
    darkPalette.setColor(QPalette::ToolTipText, QColor("#FFFFFF"));
    darkPalette.setColor(QPalette::Text, QColor("#FFFFFF"));
    darkPalette.setColor(QPalette::Button, QColor("#3E3D32"));
    darkPalette.setColor(QPalette::ButtonText, QColor("#FFFFFF"));
    darkPalette.setColor(QPalette::BrightText, QColor("#FFFFFF"));
    darkPalette.setColor(QPalette::Link, QColor("#d6d6c7"));
    darkPalette.setColor(QPalette::Highlight, QColor("#66D9EF")); // Monokai accent
    darkPalette.setColor(QPalette::HighlightedText, QColor("#FFFFFF"));
    application->setPalette(darkPalette);
    application->setStyle(QStyleFactory::create("Fusion"));
}

void ThemeBrick::applyLightTheme() {
    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor("#F5F5F5")); // Flat-Color-GTK light
    lightPalette.setColor(QPalette::WindowText, QColor("#000000"));
    lightPalette.setColor(QPalette::Base, QColor("#F5F5F5"));
    lightPalette.setColor(QPalette::AlternateBase, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::ToolTipBase, QColor("#000000"));
    lightPalette.setColor(QPalette::ToolTipText, QColor("#000000"));
    lightPalette.setColor(QPalette::Text, QColor("#000000"));
    lightPalette.setColor(QPalette::Button, QColor("#F5F5F5"));
    lightPalette.setColor(QPalette::ButtonText, QColor("#000000"));
    lightPalette.setColor(QPalette::BrightText, QColor("#FF0000"));
    lightPalette.setColor(QPalette::Link, QColor("#0078D7")); // Flat-Color-GTK accent
    lightPalette.setColor(QPalette::Highlight, QColor("#66D9EF")); // Monokai accent
    lightPalette.setColor(QPalette::HighlightedText, QColor("#000000"));
    application->setPalette(lightPalette);
    application->setStyle(QStyleFactory::create("Fusion"));
}

void ThemeBrick::disconnectThemeSignals(QWidget *widget) {
    if (widget) {
        disconnect(this, &ThemeBrick::themeChanged, widget, nullptr);
        qDebug() << "Disconnected theme signals for widget";
    }
}
