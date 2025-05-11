#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDate>

struct Task {
    int id = -1;
    QString title;
    QString description;
    QDate dueDate;
    QString priority;
    QString status;
};

#endif // TASK_H
