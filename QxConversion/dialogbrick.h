#ifndef DIALOGBRICK_H
#define DIALOGBRICK_H

#include <QObject>
#include <QString>

class DialogBrick : public QObject {
    Q_OBJECT
public:
    explicit DialogBrick(QObject *parent = nullptr);
    QString showImportDialog();
    QString showExportDialog();
};

#endif // DIALOGBRICK_H
