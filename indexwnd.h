#ifndef INDEXWND_H
#define INDEXWND_H

#include "choosecategory.h"
#include "taskdialog.h"
#include "taskmodel.h"
#include "database.h"
#include "taskui.h"
#include "taskinfo.h"
#include "navigationbar.h"

#include <QWidget>


class IndexWnd : public QWidget
{
    Q_OBJECT
public:
    IndexWnd(QWidget* parent = nullptr);
    ~IndexWnd();

    // Initialization
    void initDatabase();
    void initSearch();
    void initSortTags();

    void onDeleteTask(const int row);

    void showTaskDialog();

signals:
    void switchToCalendar();
    void switchToFocus();

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

#endif // INDEXWND_H
