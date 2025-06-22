#ifndef TIMEPICKERWIDGET_H
#define TIMEPICKERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QEvent>
#include <QMouseEvent>
#include <QDate>
#include <QDialog>

class ScrollSelector : public QWidget {
    Q_OBJECT

public:
    ScrollSelector(const QStringList& values, QWidget* parent = nullptr);

    void setCurrentIndex(int index);
    int currentIndex() const;
    QString currentText() const;

signals:
    void valueChanged(int index);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent*) override;

private:
    QStringList items;

    int current;
    int startY;
    int offsetY;

    QLabel* prevLabel;
    QLabel* currentLabel;
    QLabel* nextLabel;

    bool isDragging;

    void updateLabels();
    void animateToNearest();
};

class TimePickerWidget : public QDialog {

    Q_OBJECT
public:
    TimePickerWidget(const QDate& date, QWidget* parent = nullptr);

signals:
    void timeChosen(QDate& date, int hour, int minute, bool isAm);
    void canceled();

private:
    ScrollSelector* hourSelector;
    ScrollSelector* minuteSelector;
    ScrollSelector* ampmSelector;

    QDate date;
};

#endif // TIMEPICKERWIDGET_H
