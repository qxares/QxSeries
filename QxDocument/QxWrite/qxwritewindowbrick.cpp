#include "qxwritewindowbrick.h"
#include <QDebug>
#include <QContextMenuEvent>
#include <QMenuBar>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTextEdit>
#include <QDockWidget>
#include <QSlider>
#include <QHBoxLayout>
#include "../../QxCentre/mainwindowbrick.h"

QxWriteWindowBrick::QxWriteWindowBrick(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("QxWrite");
    setupMenus();
    setupCentralWidget();
    setupZoomDock();
    MainWindowBrick *mainWindow = qobject_cast<MainWindowBrick*>(parent);
    if (mainWindow) {
        themeBrick = mainWindow->getThemeBrick();
        interlinkBrick = mainWindow->getInterlinkBrick();
        if (interlinkBrick) {
            interlinkBrick->registerAppWindow(this);
            connect(interlinkBrick, &InterlinkBrick::windowStateChanged, this, &QxWriteWindowBrick::handleWindowStateChange);
        }
        connect(themeBrick, &ThemeBrick::themeChanged, this, &QxWriteWindowBrick::applyTheme);
        initializeTheme(themeBrick->isDarkTheme());
    }
    qDebug() << "QxWrite window initialized";
}

void QxWriteWindowBrick::setupMenus() {
    fileMenu = menuBar()->addMenu("&File");
    newAction = fileMenu->addAction("New");
    fileMenu->addAction("Open");
    fileMenu->addAction("Save");
    fileMenu->addAction("Save As");
    fileMenu->addAction("Save All");
    fileMenu->addAction("Close");
    fileMenu->addAction("Close All");
    QMenu *importMenu = fileMenu->addMenu("Import");
    importMenu->addAction("Docx");
    importMenu->addAction("ODT");
    importMenu->addAction("RTF");
    importMenu->addAction("TXT");
    importMenu->addAction("Markdown");
    QMenu *exportMenu = fileMenu->addMenu("Export");
    exportMenu->addAction("Docx");
    exportMenu->addAction("ODT");
    exportMenu->addAction("RTF");
    exportMenu->addAction("TXT");
    exportMenu->addAction("Markdown");
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
    pasteSpecialMenu->addAction("Plain Text");
    pasteSpecialMenu->addAction("Formatted Text");
    pasteSpecialMenu->addAction("HTML");
    editMenu->addAction("Select All");
    editMenu->addAction("Find");
    editMenu->addAction("Replace");
    editMenu->addAction("Go to Line");

    formatMenu = menuBar()->addMenu("&Format");
    formatMenu->addAction("Bold");
    formatMenu->addAction("Italic");
    formatMenu->addAction("Underline");
    formatMenu->addAction("Strikethrough");
    formatMenu->addAction("Superscript");
    formatMenu->addAction("Subscript");
    formatMenu->addAction("Font");
    formatMenu->addAction("Font Size");
    formatMenu->addAction("Text Color");
    formatMenu->addAction("Background Color");
    formatMenu->addAction("Align Left");
    formatMenu->addAction("Align Center");
    formatMenu->addAction("Align Right");
    formatMenu->addAction("Justify");
    QMenu *lineSpacingMenu = formatMenu->addMenu("Line Spacing");
    lineSpacingMenu->addAction("Single");
    lineSpacingMenu->addAction("1.15");
    lineSpacingMenu->addAction("1.5");
    lineSpacingMenu->addAction("Double");
    QMenu *paragraphStylesMenu = formatMenu->addMenu("Paragraph Styles");
    paragraphStylesMenu->addAction("Normal");
    paragraphStylesMenu->addAction("Heading 1");
    paragraphStylesMenu->addAction("Heading 2");
    paragraphStylesMenu->addAction("Custom");
    formatMenu->addAction("Clear Formatting");

    tableMenu = menuBar()->addMenu("&Table");
    tableMenu->addAction("Insert Table");
    tableMenu->addAction("Delete Table");
    tableMenu->addAction("Add Row Above");
    tableMenu->addAction("Add Row Below");
    tableMenu->addAction("Add Column Left");
    tableMenu->addAction("Add Column Right");
    tableMenu->addAction("Remove Row");
    tableMenu->addAction("Remove Column");
    tableMenu->addAction("Merge Cells");
    tableMenu->addAction("Split Cells");
    QMenu *tablePropsMenu = tableMenu->addMenu("Table Properties");
    tablePropsMenu->addAction("Border Style");
    tablePropsMenu->addAction("Cell Padding");
    tableMenu->addAction("Background Color");
    QMenu *sheetIntegrationMenu = tableMenu->addMenu("QxSheet Integration");
    sheetIntegrationMenu->addAction("Insert QxSheet Table");
    sheetIntegrationMenu->addAction("Edit in QxSheet");
    sheetIntegrationMenu->addAction("Export to QxSheet");

    insertMenu = menuBar()->addMenu("&Insert");
    insertMenu->addAction("Image");
    insertMenu->addAction("Hyperlink");
    insertMenu->addAction("Bookmark");
    insertMenu->addAction("Comment");
    insertMenu->addAction("Page Break");
    insertMenu->addAction("Header");
    insertMenu->addAction("Footer");
    insertMenu->addAction("Page Number");
    insertMenu->addAction("Date/Time");

    viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction("Zoom In");
    viewMenu->addAction("Zoom Out");
    viewMenu->addAction("100%");
    viewMenu->addAction("Full Screen");
    viewMenu->addAction("Show Ruler");
    viewMenu->addAction("Show Grid");
    viewMenu->addAction("Word Wrap");
    viewMenu->addAction("Document Outline");

    toolsMenu = menuBar()->addMenu("&Tools");
    toolsMenu->addAction("Spell Check");
    toolsMenu->addAction("Word Count");
    toolsMenu->addAction("Track Changes");
    toolsMenu->addAction("Compare Documents");
    toolsMenu->addAction("Preferences");
    toolsMenu->addAction("Macros");

    helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction("About QxWrite");
    helpMenu->addAction("Documentation");
    helpMenu->addAction("Keyboard Shortcuts");
    helpMenu->addAction("Check for Updates");

    connect(newAction, &QAction::triggered, this, &QxWriteWindowBrick::createNewDocument);
}

void QxWriteWindowBrick::setupCentralWidget() {
    mdiArea = new QMdiArea(this);
    mdiArea->setViewMode(QMdiArea::SubWindowView);
    mdiArea->setOption(QMdiArea::DontMaximizeSubWindowOnActivation, true);
    setCentralWidget(mdiArea);
    connect(mdiArea, &QMdiArea::subWindowActivated, this, &QxWriteWindowBrick::resizeSubWindow);
    createNewDocument();
}

void QxWriteWindowBrick::setupZoomDock() {
    QDockWidget *zoomDock = new QDockWidget("Zoom", this);
    zoomDock->setAllowedAreas(Qt::BottomDockWidgetArea);
    zoomDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    QWidget *zoomWidget = new QWidget(zoomDock);
    QHBoxLayout *zoomLayout = new QHBoxLayout(zoomWidget);
    zoomSlider = new QSlider(Qt::Horizontal, zoomWidget);
    zoomSlider->setRange(50, 200);
    zoomSlider->setValue(100);
    zoomSlider->setFixedWidth(100);
    zoomLayout->addWidget(zoomSlider);
    zoomLayout->addStretch();
    zoomDock->setWidget(zoomWidget);
    addDockWidget(Qt::BottomDockWidgetArea, zoomDock);
    connect(zoomSlider, &QSlider::valueChanged, this, &QxWriteWindowBrick::updateZoom);
}

void QxWriteWindowBrick::createNewDocument() {
    QTextEdit *textEdit = new QTextEdit();
    QMdiSubWindow *subWindow = mdiArea->addSubWindow(textEdit);
    subWindow->setWindowFlags(Qt::SubWindow | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    subWindow->setWindowTitle("Document " + QString::number(mdiArea->subWindowList().size()));
    subWindow->resize(mdiArea->viewport()->size());
    subWindow->showMaximized();
    mdiArea->setActiveSubWindow(subWindow);
}

void QxWriteWindowBrick::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    for (QMdiSubWindow *subWindow : mdiArea->subWindowList()) {
        if (subWindow->isMaximized()) {
            subWindow->resize(mdiArea->viewport()->size());
        }
    }
}

void QxWriteWindowBrick::resizeSubWindow(QMdiSubWindow *subWindow) {
    if (subWindow && subWindow->isMaximized()) {
        subWindow->resize(mdiArea->viewport()->size());
    }
}

void QxWriteWindowBrick::updateZoom(int value) {
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow()) {
        if (QTextEdit *textEdit = qobject_cast<QTextEdit*>(activeSubWindow->widget())) {
            QFont font = textEdit->font();
            font.setPointSizeF(12.0 * value / 100.0);
            textEdit->setFont(font);
        }
    }
}

void QxWriteWindowBrick::initializeTheme(bool dark) {
    applyTheme(dark);
}

void QxWriteWindowBrick::applyTheme(bool dark) {
    QPalette palette = qApp->palette();
    setPalette(palette);
    mdiArea->setPalette(palette);
    for (QMdiSubWindow *subWindow : mdiArea->subWindowList()) {
        if (QWidget *widget = subWindow->widget()) {
            widget->setPalette(palette);
        }
    }
    qDebug() << "QxWrite theme applied: " << (dark ? "dark" : "light");
}

void QxWriteWindowBrick::contextMenuEvent(QContextMenuEvent *event) {
    QMenu contextMenu(this);
    contextMenu.addAction("Cut");
    contextMenu.addAction("Copy");
    contextMenu.addAction("Paste");
    contextMenu.addAction("Bold");
    contextMenu.addAction("Italic");
    contextMenu.addAction("Underline");
    contextMenu.addAction("Insert Table");
    contextMenu.addAction("Delete Table");
    contextMenu.addAction("Insert Image");
    contextMenu.addAction("Hyperlink");
    contextMenu.addAction("Find");
    contextMenu.addAction("Spell Check");
    contextMenu.exec(event->globalPos());
}

void QxWriteWindowBrick::handleWindowStateChange(bool minimized) {
    if (minimized) {
        showMinimized();
        qDebug() << "QxWrite minimized via InterlinkBrick";
    } else {
        if (windowState() & Qt::WindowMaximized) {
            showMaximized();
        } else {
            showNormal();
        }
        raise(); // Ensure QxWrite stays above QxCentre
        qDebug() << "QxWrite restored via InterlinkBrick";
    }
}
