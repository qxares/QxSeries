#include "infodialogbrick.h"
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QSettings>

InfoDialogBrick::InfoDialogBrick(const QString &title, const QString &text, QWidget *parent)
    : BaseDialogBrick(title, parent) {
    qDebug() << "InfoDialogBrick initialized for" << title;
    setFixedSize(300, 200);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel(text, this);
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    layout->addWidget(label);
    setLayout(layout);

    // Initialize theme
    QSettings settings("QxSeries", "QxCentre");
    QString theme = settings.value("theme", "dark").toString();
    initializeTheme(theme == "dark");

    // Center on screen
    centerWindow();
    settings.setValue(QString("window_position/%1").arg(title), geometry());
}

InfoDialogBrick::~InfoDialogBrick() {
    qDebug() << "InfoDialogBrick destroyed";
}
