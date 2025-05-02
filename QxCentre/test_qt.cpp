#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
int main(int argc, char *argv[]) {
QApplication app(argc, argv);
QMainWindow window;
window.setWindowTitle("Test MenuBar");
QMenuBar *menuBar = window.menuBar();
menuBar->setStyleSheet(
"QMenuBar { background-color: #3E3D32; color: #F8F8F2; font-size: 14px; padding: 5px; }"
"QMenuBar::item { padding: 5px 10px; }"
"QMenuBar::item:selected { background-color: #66D9EF; }"
"QMenu { background-color: #3E3D32; color: #F8F8F2; font-size: 12px; }"
"QMenu::item { padding: 5px 20px; }"
"QMenu::item:selected { background-color: #66D9EF; }"
);
QMenu *testMenu = menuBar->addMenu("Test");
testMenu->addAction("Item1");
testMenu->addAction("Item2");
window.setGeometry(0, 0, 1920, 1080);
window.showMaximized();
window.show();
return app.exec();
}
