#ifndef QXSHEETWINDOWBRICK_H
#define QXSHEETWINDOWBRICK_H

#include <QMainWindow>
#include <QMenu>
#include <QTableView>

class QxSheetWindowBrick : public QMainWindow {
    Q_OBJECT
public:
    explicit QxSheetWindowBrick(QWidget *parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void setupMenus();
    void setupCentralWidget();

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *dataMenu;
    QMenu *formatMenu;
    QMenu *insertMenu;
    QMenu *viewMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;
    QTableView *tableView;
};

#endif // QXSHEETWINDOWBRICK_H
