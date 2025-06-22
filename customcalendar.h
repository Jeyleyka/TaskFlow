#ifndef CUSTOMCALENDAR_H
#define CUSTOMCALENDAR_H

#include <QWidget>
#include <QDate>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLocale>

#include "timepickerwidget.h"

class CustomCalendar : public QWidget
{
    Q_OBJECT

public:
    explicit CustomCalendar(QWidget *parent = nullptr);

    void showCentered(QWidget* parent = nullptr);
signals:
    void dateSelected(const QDate& date, int hour, int minutes, bool isAm);
    void closeCalendar();

private:
    QDate currentDate;

    QLabel* monthLabel;
    QLabel* yearLabel;

    QGridLayout* daysLayout;

    QList<QPushButton*> dayButtons;

    void updateCalendar();
    void clearCalendar();

private slots:
    void onPrevMonth();
    void onNextMonth();
    // void onDayClicked();
};

#endif // CUSTOMCALENDAR_H
