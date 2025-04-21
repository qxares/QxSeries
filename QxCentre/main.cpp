#include "mainwindowbrick.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle("Fusion");
    MainWindowBrick window;
    window.show();
    return app.exec();
}
