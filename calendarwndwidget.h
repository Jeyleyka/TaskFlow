#ifndef CALENDARWNDWIDGET_H
#define CALENDARWNDWIDGET_H

#include "choosecategory.h"
#include "taskdialog.h"
#include "taskmodel.h"
#include "database.h"
#include "taskui.h"
#include "taskinfo.h"
#include "navigationbar.h"

#include <QWidget>

class CalendarWndWidget : public QWidget
{
    Q_OBJECT
public:
    CalendarWndWidget(QWidget* parent = nullptr);
    ~CalendarWndWidget();

    // Initialization
    void initDatabase();
    void initSortTags();

    void onDeleteTask(const int row);

signals:
    void switchToindex();

private:
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
#endif // CALENDARWNDWIDGET_H
