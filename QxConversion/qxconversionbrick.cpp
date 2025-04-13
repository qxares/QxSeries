#include "qxconversionbrick.h"
#include "cleanhtmlbrick.h"
#include "dialogbrick.h"
#include <QApplication>
#include <QProcess>
#include <QFileInfo>
#include <QDebug>
#include <QClipboard>
#include <QTemporaryFile>

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
        qDebug() << "QxConversionBrick: Pandoc error:" << process.readAllStandardError();
        return false;
    }
    qDebug() << "QxConversionBrick: Pandoc succeeded with args:" << args;
    return true;
}

bool QxConversionBrick::exportFile(const QString &inputFile, const QString &outputFile) {
    QStringList formats = {".pdf", ".docx", ".html", ".epub", ".odt"};
    QString ext = QFileInfo(outputFile).suffix().toLower();
    QString newOutputFile = outputFile;

    if (!formats.contains("." + ext)) {
        newOutputFile = outputFile + ".pdf";
        qDebug() << "QxConversionBrick: Appended .pdf to outputFile:" << newOutputFile;
        ext = "pdf";
    }

    qDebug() << "QxConversionBrick: Converting to:" << newOutputFile;

    QStringList args;
    args << "--from" << "html" << "--to" << ext << "--standalone" << "--strip-comments";

    if (ext == "pdf") {
        args << "--pdf-engine=wkhtmltopdf" << "--dpi=300" << "--pdf-engine-opt=--page-size" << "--pdf-engine-opt=A4";
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
    if (ext == "epub") {
        QTemporaryFile tempMeta("XXXXXX.xml");
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

    // Use inputFile if provided, else clipboard
    QString html;
    if (!inputFile.isEmpty()) {
        QFile file(inputFile);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "QxConversionBrick: Failed to open input file:" << inputFile;
            return false;
        }
        html = QString::fromUtf8(file.readAll());
        file.close();
    } else {
        QClipboard *clipboard = QApplication::clipboard();
        html = clipboard->text();
        if (html.isEmpty()) {
            qDebug() << "QxConversionBrick: Clipboard empty";
            return false;
        }
        qDebug() << "QxConversionBrick: Using clipboard HTML";
    }

    // Clean HTML
    html = cleanHtmlBrick->cleanHtml(html);

    // Write to temp file for Pandoc
    QTemporaryFile tempFile("XXXXXX.html");
    if (!tempFile.open()) {
        qDebug() << "QxConversionBrick: Failed to create temp file";
        return false;
    }
    tempFile.write(html.toUtf8());
    tempFile.close();

    args << tempFile.fileName() << "-o" << newOutputFile;

    bool success = runPandoc(args);
    if (ext == "epub") {
        QFile::remove(args[args.indexOf("--epub-metadata") + 1]);
    }
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

    QTemporaryFile tempFile("XXXXXX.rtf");
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
