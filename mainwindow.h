#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "deletebuttondelegate.h"
#include "calendarwnd.h"
#include "indexwnd.h"
#include "focusmodewnd.h"
#include "profilewnd.h"
#include "login.h"

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
#include <QSystemTrayIcon>
#include <QMenu>

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

public slots:
    void handleLogOut();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    ChooseCategory* chooseCategory;

    TaskManager* taskManager;

    CalendarWnd* calendarWndWidget;

    IndexWnd* indexWndWidget;

    FocusModeWnd* focusWndWidget;

    ProfileWnd* profileWndWidget;

    TaskDialog* dialog = nullptr;

    QTableView* table;

    TaskModel* model;

    QStackedWidget* stackedWidget;

    DatabaseManager* dataBase;

    QVector<TaskUI*> tasks;

    QComboBox* sortTags;

    QLineEdit* search;

    QLabel* indexTitle;

    QSystemTrayIcon* trayIcon;

    QPushButton* profileImageBtn;

    QVBoxLayout* layout;
    QVBoxLayout* tasksLayout;

private slots:
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

    // QWidget* navigationBar;
};
#endif // MAINWINDOW_H
