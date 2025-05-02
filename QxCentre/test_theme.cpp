#include "test_theme.h"
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QStyleFactory>

ThemeBrick::ThemeBrick(QApplication *app)
    : QObject(app), application(app), darkThemeEnabled(true) {
    applyDarkTheme();
}

void ThemeBrick::setTheme(const QString &theme) {
    bool enableDark = (theme.toLower() == "dark");
    if (enableDark != darkThemeEnabled) {
        darkThemeEnabled = enableDark;
        if (enableDark) applyDarkTheme(); else applyLightTheme();
    }
}

void ThemeBrick::applyDarkTheme() {
    QPalette p;
    p.setColor(QPalette::Window, QColor("#272822"));
    p.setColor(QPalette::WindowText, QColor("#F8F8F2"));
    p.setColor(QPalette::Base, QColor("#272822"));
    p.setColor(QPalette::AlternateBase, QColor("#3E3D32"));
    p.setColor(QPalette::ToolTipBase, QColor("#75715E"));
    p.setColor(QPalette::ToolTipText, QColor("#F8F8F2"));
    p.setColor(QPalette::Text, QColor("#F8F8F2"));
    p.setColor(QPalette::Button, QColor("#3E3D32"));
    p.setColor(QPalette::ButtonText, QColor("#F8F8F2"));
    p.setColor(QPalette::BrightText, QColor("#F8F8F2"));
    p.setColor(QPalette::Link, QColor("#d6d6c7"));
    p.setColor(QPalette::Highlight, QColor("#3e3d33"));
    p.setColor(QPalette::HighlightedText, QColor("#F8F8F2"));
    application->setPalette(p);
    application->setStyle(QStyleFactory::create("Fusion"));
}

void ThemeBrick::applyLightTheme() {
    QPalette p;
    p.setColor(QPalette::Window, Qt::white);
    p.setColor(QPalette::WindowText, Qt::black);
    p.setColor(QPalette::Base, Qt::white);
    p.setColor(QPalette::AlternateBase, QColor(240, 240, 240));
    p.setColor(QPalette::ToolTipBase, Qt::black);
    p.setColor(QPalette::ToolTipText, Qt::black);
    p.setColor(QPalette::Text, Qt::black);
    p.setColor(QPalette::Button, Qt::white);
    p.setColor(QPalette::ButtonText, Qt::black);
    p.setColor(QPalette::BrightText, Qt::red);
    p.setColor(QPalette::Link, QColor(0, 0, 255));
    p.setColor(QPalette::Highlight, QColor(0, 120, 215));
    p.setColor(QPalette::HighlightedText, Qt::white);
    application->setPalette(p);
    application->setStyle(QStyleFactory::create("Fusion"));
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QMainWindow window;
    ThemeBrick theme(&app);
    window.setWindowTitle("Test Theme");
    QMenuBar *menuBar = window.menuBar();
    menuBar->setStyleSheet(
        "QMenuBar { background-color: #3E3D32; color: #F8F8F2; font-size: 14px; padding: 5px; }"
        "QMenuBar::item { padding: 5px 10px; }"
        "QMenuBar::item:selected { background-color: #66D9EF; }"
        "QMenu { background-color: #3E3D32; color: #F8F8F2; font-size: 12px; }"
        "QMenu::item { padding: 5px 20px; }"
        "QMenu::item:selected { background-color: #66D9EF; }"
    );
    QMenu *menu = menuBar->addMenu("Preferences");
    QMenu *themesMenu = menu->addMenu("Themes");
    QAction *darkAction = themesMenu->addAction("Dark");
    QAction *lightAction = themesMenu->addAction("Light");
    darkAction->setCheckable(true);
    lightAction->setCheckable(true);
    darkAction->setChecked(true);
    QActionGroup *themeGroup = new QActionGroup(&window);
    themeGroup->addAction(darkAction);
    themeGroup->addAction(lightAction);
    themeGroup->setExclusive(true);
    QObject::connect(darkAction, &QAction::triggered, [&theme]() { theme.setTheme("dark"); });
    QObject::connect(lightAction, &QAction::triggered, [&theme]() { theme.setTheme("light"); });
    window.setGeometry(0, 0, 1920, 1080);
    window.showMaximized();
    window.show();
    return app.exec();
}

#include "moc_test_theme.cpp"
