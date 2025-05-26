#ifndef CIRCULARTIMERWIDGET_H
#define CIRCULARTIMERWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QPaintEvent>
#include <QPainter>

class CircularTimerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CircularTimerWidget(QWidget *parent = nullptr);
    void start();
    void stop();
    bool isRunning() const;

protected:
    void paintEvent(QPaintEvent *) override;

private slots:
    void updateProgress();

private:
    QTimer timer;

    QTime startTime;

    int totalDurationSec = 0;
    int elapsedSec = 0;

    bool running;
};

#endif // CIRCULARTIMERWIDGET_H
