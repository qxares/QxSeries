#include <QApplication>
#include <QDebug>
#include "mainwindowbrick.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qDebug() << "QApplication initialized";

    MainWindowBrick *mainWindow = new MainWindowBrick;
    mainWindow->show();

    // Connect aboutToQuit for shutdown logging
    QObject::connect(&app, &QApplication::aboutToQuit, []() {
        qDebug() << "QApplication about to quit";
    });

    return app.exec();
}
