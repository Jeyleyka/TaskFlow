#ifndef TASKINFO_H
#define TASKINFO_H

#include <QDialog>
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QtSvg/QSvgRenderer>

#include "taskui.h"
#include "edittask.h"
#include "choosecategory.h"
#include "customcalendar.h"
#include "deletetaskwnd.h"
#include "Task.h"

class TaskInfo : public QDialog
{
    Q_OBJECT

public:
    explicit TaskInfo(int id, QString titleStr, QString descStr,  const QDateTime& dueDateTime, TaskUI* taskWidget, QWidget* parent = nullptr);

    const QString getTitle();
    const QString getDesc();
    const QDateTime getDueDate();
    const QString getPriority();
    const QString getCategoryName();
    const QColor getCategoryColor();
    const QIcon getCategoryIcon();

    Task getUpdatedTask() const;

    void onUpdateData();
    void loadTaskData(const Task& task);

signals:
    void onChangeUI();
    void taskDeleted(const int& taskId);

private:
    TaskUI* taskUI;
    EditTask* editTaskWnd;

    QString minutesStr;

    QVBoxLayout* mainLayout;

    QPushButton* closeTaskInfo;
    QPushButton* deleteTask;
    QPushButton* editTask;
    QPushButton* editDesc;
    QPushButton* priority;
    QPushButton* category;
    QPushButton* taskTime;

    QLabel* title;
    QLabel* description;
    QLabel* taskpriority;
    QLabel* subTask;

    QColor categoryColor;

    QDate date;

    QDateTime dateTime;

    int hours;
    int minutes;
    int Taskid;

    bool isAm;

private slots:
    void onDeleteTaskClicked();

};

#endif // TASKINFO_H
