#ifndef INFODIALOGBRICK_H
#define INFODIALOGBRICK_H

#include "basedialogbrick.h"
#include <QString>

class QLabel;

class InfoDialogBrick : public BaseDialogBrick {
    Q_OBJECT
public:
    explicit InfoDialogBrick(const QString &title, const QString &message, QWidget *parent = nullptr);
    ~InfoDialogBrick() override;

private:
    QLabel *infoLabel;
};

#endif // INFODIALOGBRICK_H
