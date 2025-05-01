#include "interlinkmaestrobrick.h"
#include <QDebug>
#include <QSettings>
#include "windowbrick.h"

InterlinkMaestroBrick::InterlinkMaestroBrick(QObject *parent)
    : QObject(parent) {
    qDebug() << "InterlinkMaestroBrick initialized";
}

InterlinkMaestroBrick::~InterlinkMaestroBrick() {
    qDebug() << "InterlinkMaestroBrick destroyed";
}

void InterlinkMaestroBrick::registerApp(const QString &appName, QWidget *window, const QStringList &dependencies) {
    AppRoadmap roadmap;
    roadmap.window = window;
    roadmap.dependencies = dependencies;
    appRoadmaps.insert(appName, roadmap);
    emit appRegistered(appName);
    qDebug() << "Registered app" << appName << "with dependencies:" << dependencies;
}

void InterlinkMaestroBrick::unregisterApp(const QString &appName) {
    if (appRoadmaps.contains(appName)) {
        appRoadmaps.remove(appName);
        emit appUnregistered(appName);
        qDebug() << "Unregistered app" << appName;
    }
}

void InterlinkMaestroBrick::updateApps(const QString &updateType) {
    for (auto it = appRoadmaps.constBegin(); it != appRoadmaps.constEnd(); ++it) {
        const QString &appName = it.key();
        const AppRoadmap &roadmap = it.value();
        if (roadmap.window && roadmap.dependencies.contains(updateType)) {
            if (updateType == "theme") {
                QSettings settings("QxSeries", "QxCentre");
                QString theme = settings.value("theme", "dark").toString();
                WindowBrick *window = qobject_cast<WindowBrick*>(roadmap.window);
                if (window) {
                    window->initializeTheme(theme == "dark");
                }
            } else if (updateType == "geometry") {
                WindowBrick *window = qobject_cast<WindowBrick*>(roadmap.window);
                if (window) {
                    window->centerWindow();
                }
            }
            emit appUpdated(appName, updateType);
            qDebug() << "Updated" << appName << "for" << updateType;
        }
    }
}
