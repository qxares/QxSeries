#ifndef QXWRITEWINDOWBRICK_H
#define QXWRITEWINDOWBRICK_H

#include <QMainWindow>
#include <QMenu>
#include <QMdiArea>
#include <QSlider>
#include <QAction>

class QxWriteWindowBrick : public QMainWindow {
    Q_OBJECT
public:
    explicit QxWriteWindowBrick(QWidget *parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateZoom(int value);
    void createNewDocument();
    void applyTheme(bool dark = false);
    void resizeSubWindow(QMdiSubWindow *subWindow);

public slots:
    void initializeTheme(bool dark);

private:
    void setupMenus();
    void setupCentralWidget();
    void setupZoomDock();

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *tableMenu;
    QMenu *insertMenu;
    QMenu *viewMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;
    QMdiArea *mdiArea;
    QSlider *zoomSlider;
    QAction *newAction;
    bool isDarkTheme;
};

#endif // QXWRITEWINDOWBRICK_H
