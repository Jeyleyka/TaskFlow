#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "taskmodel.h"
#include "taskdialog.h"
#include "database.h"

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
    QPushButton* addTaskButton;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
