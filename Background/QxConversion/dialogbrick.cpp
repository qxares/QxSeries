#include "dialogbrick.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>

DialogBrick::DialogBrick(QObject *parent) : QObject(parent) {
    qDebug() << "DialogBrick initialized";
}

QString DialogBrick::showImportDialog() {
    QFileDialog dialog(nullptr, "Import File");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Documents (*.txt *.md *.pdf *.docx *.html *.epub *.odt)");
    dialog.setStyleSheet(
        "QFileDialog {"
        "    border: 2px solid #c01c28;"
        "    background-color: #f5f5f5;"
        "}"
        "QPushButton {"
        "    border: 1px solid #c01c28;"
        "    background-color: #ffffff;"
        "    padding: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #e0e0e0;"
        "}"
    );
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);

    if (dialog.exec() == QDialog::Accepted) {
        QStringList files = dialog.selectedFiles();
        if (!files.isEmpty()) {
            QString filePath = files.first().trimmed();
            QStringList validExts = {".txt", ".md", ".pdf", ".docx", ".html", ".epub", ".odt"};
            QString ext = QFileInfo(filePath).suffix().toLower();
            if (!validExts.contains("." + ext)) {
                filePath += ".txt";
                qDebug() << "DialogBrick: Appended .txt to import filePath:" << filePath;
            }
            qDebug() << "DialogBrick: Import dialog selected:" << filePath;
            return filePath;
        }
    }
    qDebug() << "DialogBrick: Import dialog cancelled";
    return QString();
}

QString DialogBrick::showExportDialog() {
    QFileDialog dialog(nullptr, "Export File");
    dialog.setDirectory("/home/ares/test1");
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter("PDF (*.pdf);;Word (*.docx);;HTML (*.html);;EPUB (*.epub);;OpenDocument (*.odt)");
    dialog.setStyleSheet(
        "QFileDialog {"
        "    border: 2px solid #c01c28;"
        "    background-color: #f5f5f5;"
        "}"
        "QPushButton {"
        "    border: 1px solid #c01c28;"
        "    background-color: #ffffff;"
        "    padding: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #e0e0e0;"
        "}"
    );
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);

    if (dialog.exec() == QDialog::Accepted) {
        QStringList files = dialog.selectedFiles();
        if (!files.isEmpty()) {
            QString filePath = files.first().trimmed();
            QString selectedFilter = dialog.selectedNameFilter();
            QStringList validExts = {".pdf", ".docx", ".html", ".epub", ".odt"};
            QString ext = QFileInfo(filePath).suffix().toLower();

            QString defaultExt = "pdf";
            if (selectedFilter.contains("*.docx")) defaultExt = "docx";
            else if (selectedFilter.contains("*.html")) defaultExt = "html";
            else if (selectedFilter.contains("*.epub")) defaultExt = "epub";
            else if (selectedFilter.contains("*.odt")) defaultExt = "odt";

            if (!validExts.contains("." + ext)) {
                filePath += "." + defaultExt;
                qDebug() << "DialogBrick: Appended ." << defaultExt << " to export filePath:" << filePath;
            }

            if (QFileInfo(filePath).exists()) {
                QMessageBox warning;
                warning.setWindowTitle("File Exists");
                warning.setText("The file \"" + filePath + "\" already exists.");
                warning.setInformativeText("Do you want to overwrite it?");
                warning.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                warning.setDefaultButton(QMessageBox::No);
                warning.setStyleSheet(
                    "QMessageBox {"
                    "    border: 2px solid #c01c28;"
                    "    background-color: #f5f5f5;"
                    "}"
                    "QPushButton {"
                    "    border: 1px solid #c01c28;"
                    "    background-color: #ffffff;"
                    "    padding: 5px;"
                    "}"
                    "QPushButton:hover {"
                    "    background-color: #e0e0e0;"
                    "}"
                );
                if (warning.exec() != QMessageBox::Yes) {
                    qDebug() << "DialogBrick: Export dialog cancelled due to overwrite refusal";
                    return QString();
                }
            }

            qDebug() << "DialogBrick: Export dialog selected:" << filePath << "with filter:" << selectedFilter;
            return filePath;
        }
    }
    qDebug() << "DialogBrick: Export dialog cancelled";
    return QString();
}
