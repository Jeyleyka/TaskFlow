#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "taskmodel.h"
#include "taskdialog.h"
#include "database.h"
#include "deletebuttondelegate.h"

#include <QMainWindow>
#include <QTableView>
#include <QString>
#include <QCoreApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDialog>
#include <QMessageBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTableView* table;
    TaskModel* model;
    DatabaseManager* dataBase;
    QPushButton* addTaskButton;
    QPushButton* deleteTaskButton;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void onDeleteTask(const int row);
};
#endif // MAINWINDOW_H
