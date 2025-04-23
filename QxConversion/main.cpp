#include "qxconversionbrick.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QxConversionBrick brick;

    if (argc > 1 && QString(argv[1]) == "export-dialog") {
        QString filePath = brick.showExportDialog();
        if (!filePath.isEmpty()) {
            if (brick.exportFile("", filePath)) {
                qDebug() << "Main: Export completed to:" << filePath;
                return 0;
            } else {
                qDebug() << "Main: Export failed for:" << filePath;
                return 1;
            }
        }
        qDebug() << "Main: Export dialog cancelled";
        return 0;
    }

    qDebug() << "Main: Invalid or no arguments provided";
    return 0;
}
