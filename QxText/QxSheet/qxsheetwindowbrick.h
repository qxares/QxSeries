#ifndef QXSHEETWINDOWBRICK_H
#define QXSHEETWINDOWBRICK_H

#include <QMainWindow>
#include <QMenu>
#include <QTableView>
#include "../../QxCentre/themebrick.h"
#include "../../QxCentre/interlinkbrick.h"

class QxSheetWindowBrick : public QMainWindow {
    Q_OBJECT
public:
    explicit QxSheetWindowBrick(QWidget *parent = nullptr);

public slots:
    void initializeTheme(bool dark);

private slots:
    void handleWindowStateChange(bool minimized);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void setupMenus();
    void setupCentralWidget();
    void applyTheme(bool dark);
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *dataMenu;
    QMenu *formatMenu;
    QMenu *insertMenu;
    QMenu *viewMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;
    QTableView *tableView;
    ThemeBrick *themeBrick;
    InterlinkBrick *interlinkBrick;
};

#endif // QXSHEETWINDOWBRICK_H
