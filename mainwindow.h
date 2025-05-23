#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "deletebuttondelegate.h"
#include "calendarwndwidget.h"
#include "indexwndwidget.h"

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
#include <QStackedWidget>

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
    // void initNavigationBar();

    void onDeleteTask(const int row);

private:
    ChooseCategory* chooseCategory;

    CalendarWndWidget* calendarWndWidget;

    IndexWndWidget* indexWndWidget;

    TaskDialog* dialog = nullptr;

    QTableView* table;

    TaskModel* model;

    QStackedWidget* stackedWidget;

    DatabaseManager* dataBase;

    QVector<TaskUI*> tasks;

    QComboBox* sortTags;

    QLineEdit* search;

    QLabel* indexTitle;

    QPushButton* profileImageBtn;

    QVBoxLayout* layout;
    QVBoxLayout* tasksLayout;

    // QWidget* navigationBar;
};
#endif // MAINWINDOW_H
