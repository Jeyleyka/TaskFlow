#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDate>
#include <QString>
#include <QDateTime>

struct Task {
    int id = -1;
    QString title;
    QString description;
    QDateTime dueDate;
    QString priority;
    QString status;

    QString formatDateTime(const QDateTime& dt) const {
        QDate today = QDate::currentDate();
        if (dt.date() == today) {
            return "Today at " + dt.time().toString("HH:mm");
        } else {
            return dt.toString("dd.MM.yyyy 'at' HH:mm");
        }
    }
};

#endif // TASK_H
