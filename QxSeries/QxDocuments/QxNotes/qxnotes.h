#ifndef QXNOTES_H
#define QXNOTES_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTextEdit>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextToSpeech>
#include <QCloseEvent>
#include "themebrick.h"
#include "databasebrick.h"

class QxNotes : public QMainWindow {
    Q_OBJECT
public:
    explicit QxNotes(QWidget *parent = nullptr);
    ~QxNotes();

protected:
    void closeEvent(QCloseEvent *event) override;
    void moveEvent(QMoveEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupUI();
    void createMenus();
    void createTab();
    void saveTab(int index);
    void autosave();
    void updateWordCount();
    QTabWidget *tabs;
    QStatusBar *statusBar;
    ThemeBrick *themeBrick;
    DatabaseBrick *databaseBrick;
    QTextToSpeech *speech;
    QTimer *autosaveTimer;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void translateText();
    void toggleMarkdownPreview();
    void speakText();
    void showAbout();
    void currentTabChanged();
    void textChanged();
    void closeTab(int index);
};

#endif // QXNOTES_H
