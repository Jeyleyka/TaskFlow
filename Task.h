#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDate>
#include <QString>
#include <QDateTime>
#include <QLabel>

struct Task {
    int id = -1;
    int user_id;
    QString title;
    QString description;
    QDateTime dueDate;
    int priority;
    int completed = 0;
    QString categoryName;
    QColor categoryColor;
    QIcon categoryIcon;

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
