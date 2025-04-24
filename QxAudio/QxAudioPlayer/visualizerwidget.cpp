#include "visualizerwidget.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QDebug>

VisualizerWidget::VisualizerWidget(QWidget *parent) : QWidget(parent) {
    setFixedSize(100, 100);
    barHeights = QVector<float>(10, 0.0f);
    barColor = Qt::blue; // Default, updated by ThemeBrick
    backgroundColor = Qt::black;
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &VisualizerWidget::updateBars);
    animationTimer->start(100); // Update every 100ms
}

void VisualizerWidget::setThemeColors(const QColor &newBarColor, const QColor &newBackgroundColor) {
    barColor = newBarColor;
    backgroundColor = newBackgroundColor;
    update();
}

void VisualizerWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw background
    painter.fillRect(rect(), backgroundColor);

    // Draw 10 bars
    int barWidth = width() / 12; // 10 bars + spacing
    int spacing = barWidth / 2;
    for (int i = 0; i < 10; ++i) {
        int x = i * (barWidth + spacing) + spacing;
        int barHeight = barHeights[i] * height();
        painter.fillRect(x, height() - barHeight, barWidth, barHeight, barColor);
    }
}

void VisualizerWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    emit mouseDoubleClicked();
    QWidget::mouseDoubleClickEvent(event);
}

void VisualizerWidget::updateBars() {
    // Simulate audio amplitude (random for now)
    for (int i = 0; i < barHeights.size(); ++i) {
        int value = QRandomGenerator::global()->bounded(100, 900); // Integers 100–900
        barHeights[i] = value / 1000.0f; // Scale to 0.1–0.9
    }
    update();
}
