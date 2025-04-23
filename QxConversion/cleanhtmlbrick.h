#ifndef CLEANHTMLBRICK_H
#define CLEANHTMLBRICK_H

#include <QObject>
#include <QString>

class CleanHtmlBrick : public QObject {
    Q_OBJECT
public:
    explicit CleanHtmlBrick(QObject *parent = nullptr);
    QString cleanHtml(const QString &html);

private:
    QString css;
};

#endif // CLEANHTMLBRICK_H
