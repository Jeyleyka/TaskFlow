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
    void initAddTaskBtn();

    void onDeleteTask(const int row);

private:
    ChooseCategory* chooseCategory;

    TaskDialog* dialog = nullptr;

    QTableView* table;

    TaskModel* model;

    DatabaseManager* dataBase;

    QPushButton* addTaskButton;

    QVBoxLayout* layout;
    QVBoxLayout* tasksLayout;

    void showTaskDialog();
};
#endif // MAINWINDOW_H
