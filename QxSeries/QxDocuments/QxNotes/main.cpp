#include "qxnotes.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QxNotes window;
    window.show();
    return app.exec();
}
