#include "qxconversionbrick.h"
#include "cleanhtmlbrick.h"
#include "dialogbrick.h"
#include <QApplication>
#include <QProcess>
#include <QFileInfo>
#include <QDebug>
#include <QClipboard>
#include <QTemporaryFile>
#include <QMimeData>
#include <QThread>

QxConversionBrick::QxConversionBrick(QObject *parent)
    : QObject(parent), 
      cleanHtmlBrick(new CleanHtmlBrick(this)),
      dialogBrick(new DialogBrick(this)) {
    qDebug() << "QxConversionBrick initialized";
}

bool QxConversionBrick::runPandoc(const QStringList &args) {
    QProcess process;
    process.start("pandoc", args);
    if (!process.waitForFinished(30000)) {
        qDebug() << "QxConversionBrick: Pandoc failed:" << process.errorString();
        return false;
    }
    if (process.exitCode() != 0) {
        QString errorOutput = QString::fromUtf8(process.readAllStandardError());
        qDebug() << "QxConversionBrick: Pandoc error, exit code:" << process.exitCode() << "details:" << errorOutput;
        return false;
    }
    qDebug() << "QxConversionBrick: Pandoc succeeded with args:" << args;
    return true;
}

bool QxConversionBrick::exportFile(const QString &inputFile, const QString &outputFile) {
    QStringList formats = {".pdf", ".docx", ".html", ".epub", ".odt", ".rtf"};
    QString ext = QFileInfo(outputFile).suffix().toLower();
    QString newOutputFile = outputFile;

    if (!formats.contains("." + ext)) {
        newOutputFile = outputFile + ".pdf";
        qDebug() << "QxConversionBrick: Appended .pdf to outputFile:" << newOutputFile;
        ext = "pdf";
    }

    qDebug() << "QxConversionBrick: Converting to:" << newOutputFile;

    QStringList args;
    args << "--from" << "html" << "--to" << ext << "--preserve-tabs" << "--no-highlight";

    // Write CSS to temp file for table borders (non-RTF)
    QTemporaryFile cssFile("/tmp/qxconv_XXXXXX.css");
    if (ext != "rtf") {
        cssFile.setAutoRemove(false);
        if (!cssFile.open()) {
            qDebug() << "QxConversionBrick: Failed to create temp CSS file";
            return false;
        }
        QString css = "body { font-family: Liberation Sans, sans-serif; font-size: 14pt; margin: 0; padding: 0; }\n"
                      "p { margin: 0; padding: 0; }\n"
                      "table { border-collapse: collapse; width: auto; margin: 0; padding: 0; font-size: 14pt; border: 1px solid black; }\n"
                      "td, th { border: 1px solid black; padding: 5px; text-align: left; font-size: 14pt; font-family: Liberation Sans; }\n"
                      "strong { font-weight: bold; }\n"
                      "em { font-style: italic; }\n";
        cssFile.write(css.toUtf8());
        cssFile.close();
        args << "--css" << cssFile.fileName();
    }

    if (ext == "pdf") {
        args << "--pdf-engine=wkhtmltopdf" << "--dpi=300" << "--pdf-engine-opt=--page-size" << "--pdf-engine-opt=A4"
             << "--metadata" << "geometry:margin=10mm";
    }
    if (ext == "html" || ext == "epub") {
        args << "--self-contained";
    }
    if (ext == "docx" || ext == "odt") {
        QString refDoc = ext == "docx" ? "/home/ares/QxSeries/templates/reference.docx" : "/home/ares/QxSeries/templates/reference.odt";
        if (!QFileInfo(refDoc).exists()) {
            qDebug() << "QxConversionBrick: Reference doc missing:" << refDoc;
            return false;
        }
        args << "--reference-doc" << refDoc;
    }
    if (ext == "docx") {
        args << "--metadata" << "tables=true";
    }
    if (ext == "odt" || ext == "rtf") {
        args << "--metadata" << "fontsize=14pt";
    }
    if (ext == "epub") {
        QTemporaryFile tempMeta("/tmp/qxconv_XXXXXX.xml");
        tempMeta.setAutoRemove(false);
        if (!tempMeta.open()) {
            qDebug() << "QxConversionBrick: Failed to create temp metadata file";
            return false;
        }
        QString meta = "<metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\">";
        meta += "</metadata>";
        tempMeta.write(meta.toUtf8());
        tempMeta.close();
        args << "--epub-metadata" << tempMeta.fileName();
    }

    // Use inputFile if provided, else clipboard or temp file
    QString html;
    QString pandocInputFile = inputFile;
    if (!inputFile.isEmpty()) {
        QFile file(inputFile);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "QxConversionBrick: Failed to open input file:" << inputFile;
            return false;
        }
        html = QString::fromUtf8(file.readAll());
        file.close();
        qDebug() << "QxConversionBrick: Read input file, HTML length:" << html.length();
    } else {
        QClipboard *clipboard = QApplication::clipboard();
        QApplication::processEvents(); // Sync clipboard
        html = clipboard->mimeData()->html();
        qDebug() << "QxConversionBrick: Clipboard HTML length:" << html.length();
        if (html.isEmpty()) {
            qDebug() << "QxConversionBrick: Clipboard HTML empty, retrying after delay";
            QThread::msleep(300); // Wait longer
            QApplication::processEvents();
            html = clipboard->mimeData()->html();
            qDebug() << "QxConversionBrick: Clipboard HTML length after retry:" << html.length();
        }
        if (html.isEmpty()) {
            qDebug() << "QxConversionBrick: Clipboard HTML still empty, using temp file fallback";
            QTemporaryFile tempInput("/tmp/qxconv_XXXXXX.html");
            tempInput.setAutoRemove(false);
            if (!tempInput.open()) {
                qDebug() << "QxConversionBrick: Failed to create temp input file";
                return false;
            }
            html = QApplication::clipboard()->text(); // Fallback to plain text
            if (html.isEmpty()) {
                qDebug() << "QxConversionBrick: No content available for export";
                return false;
            }
            tempInput.write(html.toUtf8());
            tempInput.close();
            QFile tempFile(tempInput.fileName());
            if (!tempFile.open(QIODevice::ReadOnly)) {
                qDebug() << "QxConversionBrick: Failed to read temp input file:" << tempInput.fileName();
                return false;
            }
            html = QString::fromUtf8(tempFile.readAll());
            tempFile.close();
            qDebug() << "QxConversionBrick: Temp file HTML length:" << html.length();
            pandocInputFile = tempInput.fileName();
        } else {
            qDebug() << "QxConversionBrick: Using clipboard HTML";
        }
    }

    // Clean HTML
    html = cleanHtmlBrick->cleanHtml(html);
    qDebug() << "QxConversionBrick: Cleaned HTML length:" << html.length();

    // Write to temp file for Pandoc
    QTemporaryFile tempFile("/tmp/qxconv_XXXXXX.html");
    tempFile.setAutoRemove(false);
    if (!tempFile.open()) {
        qDebug() << "QxConversionBrick: Failed to create temp file";
        return false;
    }
    tempFile.write(html.toUtf8());
    tempFile.close();
    qDebug() << "QxConversionBrick: Wrote temp file:" << tempFile.fileName();

    // Debug temp file content
    QFile tempCheck(tempFile.fileName());
    if (tempCheck.open(QIODevice::ReadOnly)) {
        QString tempContent = QString::fromUtf8(tempCheck.readAll());
        qDebug() << "QxConversionBrick: Temp file content preview:" << tempContent.left(200);
        tempCheck.close();
    }

    args << (pandocInputFile.isEmpty() ? tempFile.fileName() : pandocInputFile) << "-o" << newOutputFile;

    bool success = runPandoc(args);
    if (ext == "epub") {
        QFile::remove(args[args.indexOf("--epub-metadata") + 1]);
    }
    if (ext != "rtf") {
        QFile::remove(cssFile.fileName());
    }
    QFile::remove(tempFile.fileName());
    if (success) {
        qDebug() << "QxConversionBrick: Exported to:" << newOutputFile;
        return true;
    }
    return false;
}

bool QxConversionBrick::importFile(const QString &inputFile) {
    QStringList formats = {".txt", ".md", ".pdf", ".docx", ".html", ".epub", ".odt"};
    QString ext = QFileInfo(inputFile).suffix().toLower();
    if (!formats.contains("." + ext)) {
        qDebug() << "QxConversionBrick: Unsupported import format:" << ext;
        return false;
    }

    QString pandocFrom = ext == "txt" ? "plain" : ext;
    QStringList args;
    args << "--from" << pandocFrom << "--to" << "rtf" << "--standalone" << inputFile;

    QTemporaryFile tempFile("/tmp/qxconv_XXXXXX.rtf");
    tempFile.setAutoRemove(false);
    if (!tempFile.open()) {
        qDebug() << "QxConversionBrick: Failed to create temp RTF file";
        return false;
    }
    QString tempPath = tempFile.fileName();
    tempFile.close();

    args << "-o" << tempPath;

    if (runPandoc(args)) {
        QFile file(tempPath);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "QxConversionBrick: Failed to read temp RTF file";
            return false;
        }
        QString rtf = QString::fromUtf8(file.readAll());
        file.close();
        QFile::remove(tempPath);

        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(rtf);
        qDebug() << "QxConversionBrick: RTF copied to clipboard";
        return true;
    }
    return false;
}

QString QxConversionBrick::showImportDialog() {
    return dialogBrick->showImportDialog();
}

QString QxConversionBrick::showExportDialog() {
    return dialogBrick->showExportDialog();
}
