#ifndef QXCONVERSIONBRICK_H
#define QXCONVERSIONBRICK_H

#include <QObject>

class CleanHtmlBrick;
class DialogBrick;

class QxConversionBrick : public QObject {
    Q_OBJECT
public:
    explicit QxConversionBrick(QObject *parent = nullptr);
    bool exportFile(const QString &inputFile, const QString &outputFile);
    bool importFile(const QString &inputFile);
    QString showImportDialog();
    QString showExportDialog();

private:
    bool runPandoc(const QStringList &args);
    CleanHtmlBrick *cleanHtmlBrick;
    DialogBrick *dialogBrick;
};

#endif // QXCONVERSIONBRICK_H
