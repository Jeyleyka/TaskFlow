#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QWidget>
#include <QDate>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QPushButton>
#include <QLabel>

class CalendarWidget : public QWidget
{
    Q_OBJECT

public:
    CalendarWidget(QWidget* parent = nullptr);

signals:
    void dateSelected(const QDate& date);

private:
    QHBoxLayout* headerLayout;
    QHBoxLayout* daysLayout;
    QLabel* monthLabel;
    QLabel* yearLabel;
    QDate currentDate;
    QDate selectedDate;
    QScrollArea* scrollArea;

    void updateCalendar();
    void highlightToday(QPushButton* btn, const QDate& date);
    void highlightWeekend(QPushButton* btn, const QDate& date);
};

#endif // CALENDARWIDGET_H
