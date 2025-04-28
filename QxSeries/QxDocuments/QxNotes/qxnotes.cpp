#include "qxnotes.h"
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QTimer>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QSplitter>
#include <QRegularExpression>

QxNotes::QxNotes(QWidget *parent) : QMainWindow(parent) {
    themeBrick = new ThemeBrick(qApp, this);
    databaseBrick = new DatabaseBrick(this);
    speech = new QTextToSpeech(this);
    autosaveTimer = new QTimer(this);

    if (!databaseBrick->initialize()) {
        qDebug() << "Failed to initialize DatabaseBrick";
    }

    setWindowTitle("QxNotes");
    setMinimumSize(600, 400);
    QString theme = databaseBrick->loadTheme();
    themeBrick->toggleDarkTheme(theme == "dark");

    int x, y, width, height;
    if (databaseBrick->loadWindowPosition("QxNotes", x, y, width, height)) {
        setGeometry(x, y, width, height);
    } else {
        resize(800, 600);
    }

    setupUI();
    createMenus();
    createTab();
    connect(autosaveTimer, &QTimer::timeout, this, &QxNotes::autosave);
    autosaveTimer->start(120000); // 2 minutes
    qDebug() << "QxNotes initialized";
}

QxNotes::~QxNotes() {
    autosave();
    delete themeBrick;
    delete databaseBrick;
    delete speech;
    qDebug() << "QxNotes destroyed";
}

void QxNotes::setupUI() {
    tabs = new QTabWidget(this);
    tabs->setTabsClosable(true);
    tabs->setMovable(true);
    setCentralWidget(tabs);
    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    connect(tabs, &QTabWidget::currentChanged, this, &QxNotes::currentTabChanged);
    connect(tabs, &QTabWidget::tabCloseRequested, this, &QxNotes::closeTab);
}

void QxNotes::createMenus() {
    fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("New", this, &QxNotes::newFile, QKeySequence::New);
    fileMenu->addAction("Open", this, &QxNotes::openFile, QKeySequence::Open);
    fileMenu->addAction("Save", this, &QxNotes::saveFile, QKeySequence::Save);
    fileMenu->addAction("Save As", this, &QxNotes::saveAsFile);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QMainWindow::close);

    editMenu = menuBar()->addMenu("Edit");
    editMenu->addAction("Undo", this, &QxNotes::undo, QKeySequence::Undo);
    editMenu->addAction("Redo", this, &QxNotes::redo, QKeySequence::Redo);
    editMenu->addSeparator();
    editMenu->addAction("Cut", this, &QxNotes::cut, QKeySequence::Cut);
    editMenu->addAction("Copy", this, &QxNotes::copy, QKeySequence::Copy);
    editMenu->addAction("Paste", this, &QxNotes::paste, QKeySequence::Paste);

    toolsMenu = menuBar()->addMenu("Tools");
    toolsMenu->addAction("Translate", this, &QxNotes::translateText);
    toolsMenu->addAction("Markdown Preview", this, &QxNotes::toggleMarkdownPreview)->setCheckable(true);
    toolsMenu->addAction("Text-to-Speech", this, &QxNotes::speakText);

    helpMenu = menuBar()->addMenu("Help");
    helpMenu->addAction("About QxNotes", this, &QxNotes::showAbout);
}

void QxNotes::createTab() {
    QWidget *tabWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tabWidget);
    QTextEdit *textEdit = new QTextEdit();
    textEdit->setFont(QFont("Arial", 12));
    layout->addWidget(textEdit);
    tabs->addTab(tabWidget, "Untitled");
    connect(textEdit, &QTextEdit::textChanged, this, &QxNotes::textChanged);
    updateWordCount();
}

void QxNotes::newFile() {
    createTab();
}

void QxNotes::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open file");
        return;
    }
    QTextStream in(&file);
    QWidget *tabWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tabWidget);
    QTextEdit *textEdit = new QTextEdit();
    textEdit->setFont(QFont("Arial", 12));
    textEdit->setText(in.readAll());
    layout->addWidget(textEdit);
    tabs->addTab(tabWidget, QFileInfo(fileName).fileName());
    tabs->setTabToolTip(tabs->count() - 1, fileName);
    file.close();
    connect(textEdit, &QTextEdit::textChanged, this, &QxNotes::textChanged);
    updateWordCount();
}

void QxNotes::saveFile() {
    int index = tabs->currentIndex();
    if (index < 0) return;
    QString fileName = tabs->tabToolTip(index);
    if (fileName.isEmpty()) {
        saveAsFile();
    } else {
        saveTab(index);
    }
}

void QxNotes::saveAsFile() {
    int index = tabs->currentIndex();
    if (index < 0) return;
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;
    tabs->setTabToolTip(index, fileName);
    tabs->setTabText(index, QFileInfo(fileName).fileName());
    saveTab(index);
}

void QxNotes::saveTab(int index) {
    QString fileName = tabs->tabToolTip(index);
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot save file");
        return;
    }
    QTextStream out(&file);
    QWidget *tabWidget = tabs->widget(index);
    QTextEdit *textEdit = tabWidget->findChild<QTextEdit*>();
    out << textEdit->toPlainText();
    file.close();
}

void QxNotes::autosave() {
    for (int i = 0; i < tabs->count(); ++i) {
        QString fileName = tabs->tabToolTip(i);
        if (!fileName.isEmpty()) {
            saveTab(i);
            qDebug() << "Autosaved tab:" << tabs->tabText(i);
        }
    }
}

void QxNotes::undo() {
    if (tabs->currentIndex() >= 0) {
        QWidget *tabWidget = tabs->currentWidget();
        QTextEdit *textEdit = tabWidget->findChild<QTextEdit*>();
        textEdit->undo();
    }
}

void QxNotes::redo() {
    if (tabs->currentIndex() >= 0) {
        QWidget *tabWidget = tabs->currentWidget();
        QTextEdit *textEdit = tabWidget->findChild<QTextEdit*>();
        textEdit->redo();
    }
}

void QxNotes::cut() {
    if (tabs->currentIndex() >= 0) {
        QWidget *tabWidget = tabs->currentWidget();
        QTextEdit *textEdit = tabWidget->findChild<QTextEdit*>();
        textEdit->cut();
    }
}

void QxNotes::copy() {
    if (tabs->currentIndex() >= 0) {
        QWidget *tabWidget = tabs->currentWidget();
        QTextEdit *textEdit = tabWidget->findChild<QTextEdit*>();
        textEdit->copy();
    }
}

void QxNotes::paste() {
    if (tabs->currentIndex() >= 0) {
        QWidget *tabWidget = tabs->currentWidget();
        QTextEdit *textEdit = tabWidget->findChild<QTextEdit*>();
        textEdit->paste();
    }
}

void QxNotes::translateText() {
    if (tabs->currentIndex() >= 0) {
        QWidget *tabWidget = tabs->currentWidget();
        QTextEdit *textEdit = tabWidget->findChild<QTextEdit*>();
        QString selectedText = textEdit->textCursor().selectedText();
        if (!selectedText.isEmpty()) {
            QString translated = selectedText.toUpper(); // Mock translation
            textEdit->insertPlainText(translated);
            qDebug() << "Translated text (mock):" << translated;
        } else {
            QMessageBox::information(this, "Translate", "Please select text to translate");
        }
    }
}

void QxNotes::toggleMarkdownPreview() {
    QAction *action = qobject_cast<QAction*>(sender());
    int index = tabs->currentIndex();
    if (index < 0) return;
    QWidget *tabWidget = tabs->widget(index);
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(tabWidget->layout());
    QTextEdit *textEdit = tabWidget->findChild<QTextEdit*>();

    if (action->isChecked()) {
        QSplitter *splitter = new QSplitter(Qt::Horizontal);
        QTextBrowser *preview = new QTextBrowser();
        preview->setFont(QFont("Arial", 12));
        layout->removeWidget(textEdit);
        splitter->addWidget(textEdit);
        splitter->addWidget(preview);
        layout->addWidget(splitter);
        QString markdown = textEdit->toPlainText();
        QString html = markdown;
        html.replace(QRegularExpression("^# (.+)$", QRegularExpression::MultilineOption), "<h1>\\1</h1>");
        html.replace(QRegularExpression("\\*([^\\*]+)\\*"), "<i>\\1</i>");
        preview->setHtml(html);
        connect(textEdit, &QTextEdit::textChanged, this, [=]() {
            QString updated = textEdit->toPlainText();
            QString updatedHtml = updated;
            updatedHtml.replace(QRegularExpression("^# (.+)$", QRegularExpression::MultilineOption), "<h1>\\1</h1>");
            updatedHtml.replace(QRegularExpression("\\*([^\\*]+)\\*"), "<i>\\1</i>");
            preview->setHtml(updatedHtml);
        });
    } else {
        QSplitter *splitter = tabWidget->findChild<QSplitter*>();
        if (splitter) {
            QTextBrowser *preview = splitter->findChild<QTextBrowser*>();
            disconnect(textEdit, &QTextEdit::textChanged, nullptr, nullptr);
            layout->removeWidget(splitter);
            layout->addWidget(textEdit);
            delete splitter;
            delete preview;
        }
    }
}

void QxNotes::speakText() {
    if (tabs->currentIndex() >= 0) {
        QWidget *tabWidget = tabs->currentWidget();
        QTextEdit *textEdit = tabWidget->findChild<QTextEdit*>();
        QString selectedText = textEdit->textCursor().selectedText();
        if (!selectedText.isEmpty()) {
            speech->say(selectedText);
        } else {
            speech->say(textEdit->toPlainText());
        }
    }
}

void QxNotes::showAbout() {
    QMessageBox::about(this, "About QxNotes", "QxNotes - A QxSeries Text Editor\nCreated by Peter van den Bosch and Grok xAI");
}

void QxNotes::currentTabChanged() {
    updateWordCount();
}

void QxNotes::textChanged() {
    updateWordCount();
}

void QxNotes::updateWordCount() {
    if (tabs->currentIndex() >= 0) {
        QWidget *tabWidget = tabs->currentWidget();
        QTextEdit *textEdit = tabWidget->findChild<QTextEdit*>();
        QString text = textEdit->toPlainText();
        int words = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts).count();
        int chars = text.length();
        statusBar->showMessage(QString("Words: %1 | Characters: %2").arg(words).arg(chars));
    } else {
        statusBar->clearMessage();
    }
}

void QxNotes::closeTab(int index) {
    QString fileName = tabs->tabToolTip(index);
    if (!fileName.isEmpty()) {
        saveTab(index);
    }
    tabs->removeTab(index);
}

void QxNotes::closeEvent(QCloseEvent *event) {
    autosave();
    event->accept();
    qDebug() << "QxNotes closeEvent accepted";
}

void QxNotes::moveEvent(QMoveEvent *event) {
    QMainWindow::moveEvent(event);
    databaseBrick->saveWindowPosition("QxNotes", x(), y(), width(), height());
}

void QxNotes::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    databaseBrick->saveWindowPosition("QxNotes", x(), y(), width(), height());
}
