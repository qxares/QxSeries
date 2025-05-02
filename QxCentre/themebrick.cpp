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
    darkPalette.setColor(QPalette::Window, QColor("#272822")); // Dark gray-green
    darkPalette.setColor(QPalette::WindowText, QColor("#F8F8F2")); // Off-white
    darkPalette.setColor(QPalette::Base, QColor("#272822")); // Dark gray-green
    darkPalette.setColor(QPalette::AlternateBase, QColor("#3e3d33")); // Dark gray
    darkPalette.setColor(QPalette::ToolTipBase, QColor("#75715E")); // Neutral
    darkPalette.setColor(QPalette::ToolTipText, QColor("#F8F8F2")); // Off-white
    darkPalette.setColor(QPalette::Text, QColor("#F8F8F2")); // Off-white
    darkPalette.setColor(QPalette::Button, QColor("#3e3d33")); // Dark gray
    darkPalette.setColor(QPalette::ButtonText, QColor("#F8F8F2")); // Off-white
    darkPalette.setColor(QPalette::BrightText, QColor("#F8F8F2")); // Off-white
    darkPalette.setColor(QPalette::Link, QColor("#d6d6c7")); // Light gray
    darkPalette.setColor(QPalette::Highlight, QColor("#66D9EF")); // Monokai accent
    darkPalette.setColor(QPalette::HighlightedText, QColor("#F8F8F2")); // Off-white
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
