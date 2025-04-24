#include <QApplication>
#include "mainwindowbrick.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontUseNativeDialogs); // Force Qt-native dialogs
    MainWindowBrick window;
    window.show();
    return app.exec();
}
