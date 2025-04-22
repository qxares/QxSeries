#include "qxsheetwindowbrick.h"
#include <QDebug>
#include <QContextMenuEvent>
#include <QMenuBar>
#include <QTableView>
#include <QStandardItemModel>

QxSheetWindowBrick::QxSheetWindowBrick(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("QxSheet");
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    setupMenus();
    setupCentralWidget();
    qDebug() << "QxSheet window initialized";
}

void QxSheetWindowBrick::setupMenus() {
    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("New");
    fileMenu->addAction("Open");
    fileMenu->addAction("Save");
    fileMenu->addAction("Save As");
    fileMenu->addAction("Save All");
    fileMenu->addAction("Close");
    fileMenu->addAction("Close All");
    QMenu *importMenu = fileMenu->addMenu("Import");
    importMenu->addAction("ODS");
    importMenu->addAction("CSV");
    importMenu->addAction("Excel");
    importMenu->addAction("JSON");
    QMenu *exportMenu = fileMenu->addMenu("Export");
    exportMenu->addAction("ODS");
    exportMenu->addAction("CSV");
    exportMenu->addAction("Excel");
    exportMenu->addAction("JSON");
    exportMenu->addAction("PDF");
    fileMenu->addAction("Print");
    fileMenu->addAction("Print Preview");
    fileMenu->addSeparator();
    fileMenu->addAction("Exit");

    editMenu = menuBar()->addMenu("&Edit");
    editMenu->addAction("Undo");
    editMenu->addAction("Redo");
    editMenu->addSeparator();
    editMenu->addAction("Cut");
    editMenu->addAction("Copy");
    editMenu->addAction("Paste");
    QMenu *pasteSpecialMenu = editMenu->addMenu("Paste Special");
    pasteSpecialMenu->addAction("Values");
    pasteSpecialMenu->addAction("Formulas");
    pasteSpecialMenu->addAction("Formats");
    pasteSpecialMenu->addAction("All");
    editMenu->addAction("Delete");
    editMenu->addAction("Select All");
    editMenu->addAction("Find");
    editMenu->addAction("Replace");
    editMenu->addAction("Go to Cell");

    dataMenu = menuBar()->addMenu("&Data");
    dataMenu->addAction("Sort Ascending");
    dataMenu->addAction("Sort Descending");
    dataMenu->addAction("Filter");
    dataMenu->addAction("Remove Duplicates");
    dataMenu->addAction("Text to Columns");
    dataMenu->addAction("Import from QxData");
    dataMenu->addAction("Export to QxData");
    QMenu *writeIntegrationMenu = dataMenu->addMenu("QxWrite Integration");
    writeIntegrationMenu->addAction("Insert in QxWrite");
    writeIntegrationMenu->addAction("Import from QxWrite");
    writeIntegrationMenu->addAction("Link to QxWrite");

    formatMenu = menuBar()->addMenu("&Format");
    QMenu *numberFormatMenu = formatMenu->addMenu("Number Format");
    numberFormatMenu->addAction("General");
    numberFormatMenu->addAction("Currency");
    numberFormatMenu->addAction("Percentage");
    numberFormatMenu->addAction("Date");
    numberFormatMenu->addAction("Custom");
    formatMenu->addAction("Bold");
    formatMenu->addAction("Italic");
    formatMenu->addAction("Underline");
    formatMenu->addAction("Font");
    formatMenu->addAction("Font Size");
    formatMenu->addAction("Cell Color");
    formatMenu->addAction("Text Color");
    formatMenu->addAction("Align Left");
    formatMenu->addAction("Align Center");
    formatMenu->addAction("Align Right");
    formatMenu->addAction("Merge Cells");
    formatMenu->addAction("Wrap Text");
    formatMenu->addAction("Conditional Formatting");
    formatMenu->addAction("Clear Formatting");

    insertMenu = menuBar()->addMenu("&Insert");
    insertMenu->addAction("Row");
    insertMenu->addAction("Column");
    insertMenu->addAction("Cell");
    QMenu *chartMenu = insertMenu->addMenu("Chart");
    chartMenu->addAction("Bar");
    chartMenu->addAction("Line");
    chartMenu->addAction("Pie");
    chartMenu->addAction("Scatter");
    insertMenu->addAction("Image");
    insertMenu->addAction("Hyperlink");
    insertMenu->addAction("Comment");
    insertMenu->addAction("Named Range");
    insertMenu->addAction("Function");

    viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction("Zoom In");
    viewMenu->addAction("Zoom Out");
    viewMenu->addAction("100%");
    viewMenu->addAction("Freeze Panes");
    viewMenu->addAction("Split Window");
    viewMenu->addAction("Show Gridlines");
    viewMenu->addAction("Show Formulas");
    viewMenu->addAction("Full Screen");

    toolsMenu = menuBar()->addMenu("&Tools");
    toolsMenu->addAction("Formula Builder");
    toolsMenu->addAction("Goal Seek");
    toolsMenu->addAction("Solver");
    toolsMenu->addAction("Data Validation");
    toolsMenu->addAction("Spell Check");
    toolsMenu->addAction("Preferences");
    toolsMenu->addAction("Macros");

    helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction("About QxSheet");
    helpMenu->addAction("Documentation");
    helpMenu->addAction("Keyboard Shortcuts");
    helpMenu->addAction("Check for Updates");
}

void QxSheetWindowBrick::setupCentralWidget() {
    tableView = new QTableView(this);
    QStandardItemModel *model = new QStandardItemModel(this);
    tableView->setModel(model);
    tableView->setStyleSheet("background-color: white; color: black;");
    setCentralWidget(tableView);
}

void QxSheetWindowBrick::contextMenuEvent(QContextMenuEvent *event) {
    QMenu contextMenu(this);
    contextMenu.addAction("Cut");
    contextMenu.addAction("Copy");
    contextMenu.addAction("Paste");
    contextMenu.addAction("Insert Row");
    contextMenu.addAction("Insert Column");
    contextMenu.addAction("Delete Row");
    contextMenu.addAction("Delete Column");
    contextMenu.addAction("Format Cells");
    contextMenu.addAction("Sort Ascending");
    contextMenu.addAction("Filter");
    contextMenu.addAction("Insert Chart");
    contextMenu.exec(event->globalPos());
}
