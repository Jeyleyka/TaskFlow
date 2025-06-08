#ifndef CALENDARWND_H
#define CALENDARWND_H

#include "choosecategory.h"
#include "taskdialog.h"
#include "taskmodel.h"
#include "database.h"
#include "taskui.h"
#include "taskinfo.h"
#include "navigationbar.h"
#include "calendarwidget.h"
#include "taskmanager.h"

#include <QWidget>
#include <QShowEvent>

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

signals:
    void switchToIndex();
    void switchToFocus();
    void switchToProfile();

// protected:
//     void showEvent(QShowEvent* event) override;

private:
    CalendarWidget* calendarWidget;

    ChooseCategory* chooseCategory;

    NavigationBar* navBar;

    TaskDialog* dialog = nullptr;

    TaskModel* model;

    TaskManager* taskManager;

    DatabaseManager* dataBase;

    QVector<TaskUI*> tasks;

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

    void showTaskDialog();
    void updateButtonStyles();

private slots:
    void clearTasksUI();
    void onTaskCreated(const Task& task);
    void onTaskUpdated(const Task& task);
    void onTaskDeleted(const int taskId);
};
#endif // CALENDARWND_H
