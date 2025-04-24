#ifndef VISUALIZERWIDGET_H
#define VISUALIZERWIDGET_H

#include <QWidget>
#include <QTimer>

class VisualizerWidget : public QWidget {
    Q_OBJECT
public:
    explicit VisualizerWidget(QWidget *parent = nullptr);
    void setThemeColors(const QColor &barColor, const QColor &backgroundColor);

signals:
    void mouseDoubleClicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private slots:
    void updateBars();

private:
    QTimer *animationTimer;
    QVector<float> barHeights;
    QColor barColor;
    QColor backgroundColor;
};

#endif // VISUALIZERWIDGET_H
