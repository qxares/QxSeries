#ifndef BASEDIALOGBRICK_H
#define BASEDIALOGBRICK_H

#include <QDialog>
#include <QString>

class ThemeBrick;
class QCloseEvent;

class BaseDialogBrick : public QDialog {
    Q_OBJECT

public:
    explicit BaseDialogBrick(const QString &title, QWidget *parent = nullptr);
    ~BaseDialogBrick();

protected:
    void initializeTheme(bool isDark);
    void centerWindow();
    void saveGeometry();
    void closeEvent(QCloseEvent *event) override;

private:
    QString title;
    ThemeBrick *themeBrick;
};

#endif // BASEDIALOGBRICK_H
