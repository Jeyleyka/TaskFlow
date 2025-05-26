#include "circulartimerwidget.h"

CircularTimerWidget::CircularTimerWidget(QWidget* parent)
    : QWidget(parent), running(false)
{
    connect(&timer, &QTimer::timeout, this, [this] {
        if (this->running) {
            this->elapsedSec++;
            this->update();
        }
    });

    this->timer.setInterval(1000);
}

void CircularTimerWidget::start() {
    if (running)
        return;

    running = true;
    this->timer.start(1000);
}

void CircularTimerWidget::stop() {
    this->running = false;
    this->timer.stop();
    this->update();
}

bool CircularTimerWidget::isRunning() const {
    return this->running;
}

void CircularTimerWidget::updateProgress() {
    if (!this->running) return;

    this->elapsedSec++;

    if (this->elapsedSec >= this->totalDurationSec)
        this->stop();

    this->update();
}

void CircularTimerWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height());
    int penWidth = 15;
    int margin = penWidth / 2;

    QRectF circleRect((width() - side) / 2.0 + margin,
                      (height() - side) / 2.0 + margin,
                      side - 2 * margin,
                      side - 2 * margin);

    // Серое кольцо
    painter.setPen(QPen(QColor(200, 200, 200), penWidth));
    painter.drawEllipse(circleRect);

    // Фиолетовая дуга (по циклу, например, каждый 60 секунд — один круг)
    int secondsPerCircle = 3600; // каждый круг — 60 секунд
    qreal angle = 360.0 * qMin(elapsedSec, secondsPerCircle) / secondsPerCircle;

    painter.setPen(QPen(QColor(150, 100, 255), penWidth));
    painter.drawArc(circleRect, 90 * 16, -angle * 16);

    // Показываем прошедшее время (часы:минуты:секунды)
    int hours = elapsedSec / 3600;
    int minutes = (elapsedSec % 3600) / 60;
    int seconds = elapsedSec % 60;

    QString timeText = QString("%2:%3")
                           .arg(minutes, 2, 10, QLatin1Char('0'))
                           .arg(seconds, 2, 10, QLatin1Char('0'));

    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(side / 8);
    painter.setFont(font);
    painter.drawText(rect(), Qt::AlignCenter, timeText);
}
