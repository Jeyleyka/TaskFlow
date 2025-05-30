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

#include <QWidget>

class CalendarWnd : public QWidget
{
    Q_OBJECT
public:
    CalendarWnd(QWidget* parent = nullptr);
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

private:
    CalendarWidget* calendarWidget;

    ChooseCategory* chooseCategory;

    NavigationBar* navBar;

    TaskDialog* dialog = nullptr;

    TaskModel* model;

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

    QVBoxLayout* layout;
    QVBoxLayout* tasksLayout;
    QVBoxLayout* searchLayout;

    QHBoxLayout* titleLayout;
    QHBoxLayout* btnsLayout;

    QWidget* btnsContainer;

    void showTaskDialog();
};
#endif // CALENDARWND_H
