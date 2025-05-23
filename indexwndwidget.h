#ifndef INDEXWNDWIDGET_H
#define INDEXWNDWIDGET_H

#include "choosecategory.h"
#include "taskdialog.h"
#include "taskmodel.h"
#include "database.h"
#include "taskui.h"
#include "taskinfo.h"
#include "navigationbar.h"

#include <QWidget>


class IndexWndWidget : public QWidget
{
    Q_OBJECT
public:
    IndexWndWidget(QWidget* parent = nullptr);
    ~IndexWndWidget();

    // Initialization
    void initDatabase();
    void initSearch();
    void initSortTags();

    void onDeleteTask(const int row);

    void showTaskDialog();

signals:
    void switchToCalendar();

private:
    ChooseCategory* chooseCategory;

    NavigationBar* navBar;

    TaskDialog* dialog = nullptr;

    TaskModel* model;

    DatabaseManager* dataBase;

    QVector<TaskUI*> tasks;
    QVector<TaskUI*> todayTasks;
    QVector<TaskUI*> completedTasks;

    QLineEdit* search;

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
    QVBoxLayout* completeTaskslayout;

    QHBoxLayout* titleLayout;

    void searchTaskFilter(const QString& title);
};

#endif // INDEXWNDWIDGET_H
