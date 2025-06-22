#ifndef CALENDARWND_H
#define CALENDARWND_H

#include "choosecategory.h"
#include "addtask.h"
#include "database.h"
#include "taskui.h"
#include "taskinfo.h"
#include "navigationbar.h"
#include "calendarwidget.h"
#include "taskmanager.h"

#include <QWidget>
#include <QShowEvent>
#include <QPointer>

class CalendarWnd : public QWidget
{
    Q_OBJECT

public:
    CalendarWnd(TaskManager* taskManager, QWidget* parent = nullptr);
    ~CalendarWnd();

    // Initialization
    void initDatabase();
    void initCalendarWidget();
    void initSortTags();

    void onDeleteTask(const int row);

public slots:
    void onTaskUpdated(const Task& task);

signals:
    void switchToIndex();
    void switchToFocus();
    void switchToProfile();
    void taskUpdated(const Task &task);
    void onDateUpdated(const QDate& date);

// protected:
//     void showEvent(QShowEvent* event) override;

private:
    CalendarWidget* calendarWidget;

    ChooseCategory* chooseCategory;

    NavigationBar* navBar;

    AddTask* addTaskWnd = nullptr;

    TaskManager* taskManager;

    DatabaseManager* dataBase;

    QVector<TaskUI*> tasks;

    QDate selectedDate;

    QLabel* indexLabel;
    QLabel* calendarLabel;
    QLabel* focusLabel;
    QLabel* profileLabel;
    QLabel* indexTitle;

    QPushButton* addTaskButton;
    QPushButton* indexBtn;
    QPushButton* calendarBtn;
    QPushButton* focusBtn;
    QPushButton* profileBtn;
    QPushButton* profileImageBtn;
    QPushButton* todayTasksBtn;
    QPushButton* completedTasksBtn;
    QPushButton* activeButton;
    QPushButton* currentActiveButton;

    QList<QPushButton*> allTaskButtons;

    QVBoxLayout* layout;
    QVBoxLayout* tasksLayout;
    QVBoxLayout* searchLayout;

    QHBoxLayout* titleLayout;
    QHBoxLayout* btnsLayout;

    QColor newColor;

    QWidget* btnsContainer;

    bool isActive;
    bool filterCompletedOnly = false;
    bool filterTodayOnly = true;

    void showTaskDialog();
    void updateButtonStyles();
    void filterTasksByDate(const QDate& date);

private slots:
    void clearTasksUI();
    void onTaskCreated(const Task& task);
    void onTaskDeleted(const int taskId);
    // void applyFilter();
};
#endif // CALENDARWND_H
