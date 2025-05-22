#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "taskmodel.h"
#include "taskdialog.h"
#include "database.h"
#include "deletebuttondelegate.h"
#include "taskui.h"
#include "taskinfo.h"
#include "choosecategory.h"

#include <QMainWindow>
#include <QTableView>
#include <QString>
#include <QCoreApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDialog>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QPoint>
#include <QFont>
#include <QComboBox>
#include <QVector>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Initialization
    void initTable();
    void initModel();
    void initDatabase();
    void initSearch();
    void initSortTags();
    void initNavigationBar();

    void onDeleteTask(const int row);

private:
    ChooseCategory* chooseCategory;

    TaskDialog* dialog = nullptr;

    QTableView* table;

    TaskModel* model;

    DatabaseManager* dataBase;

    QVector<TaskUI*> tasks;

    QComboBox* sortTags;

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

    QVBoxLayout* layout;
    QVBoxLayout* tasksLayout;

    QWidget* navigationBar;

    void showTaskDialog();
    void filterTasks(const QString& filter);
    void searchTaskFilter(const QString& title);
};
#endif // MAINWINDOW_H
