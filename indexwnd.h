#ifndef INDEXWND_H
#define INDEXWND_H

#include "choosecategory.h"
#include "taskdialog.h"
#include "taskmodel.h"
#include "database.h"
#include "taskui.h"
#include "taskinfo.h"
#include "navigationbar.h"
#include "profilewnd.h"
#include "UserSession.h"
#include "categoryeditor.h"
#include "taskmanager.h"

#include <QWidget>
#include <QScrollArea>

class IndexWnd : public QWidget
{
    Q_OBJECT
public:
    explicit IndexWnd(TaskManager* taskManager, QWidget* parent = nullptr);
    ~IndexWnd();

    // Initialization
    void initDatabase();
    void initSearch();
    void initSortTags();

    void onDeleteTask(const int row);

    void showTaskDialog();

public slots:
    void updateProfileIcon(const QPixmap& pixmap);

signals:
    void switchToCalendar();
    void switchToFocus();
    void switchToProfile();
    void updateTasks(const Task& task);

private:
    ChooseCategory* chooseCategory;

    NavigationBar* navBar;

    TaskDialog* dialog = nullptr;

    TaskModel* model;

    TaskManager* taskManager;

    ProfileWnd* profile;

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

    QScrollArea* scrollArea;
    QScrollArea* completedScroll;

    QPushButton* iconBtn;
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

private slots:
    void onTaskCreated(const Task& task);
    void onTaskUpdated(const Task& task);
    void onTaskDeleted(const int taskId);
};

#endif // INDEXWND_H
