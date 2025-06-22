#ifndef FOCUSMODEWND_H
#define FOCUSMODEWND_H

#include "navigationbar.h"
#include "database.h"
#include "addtask.h"
#include "taskui.h"
#include "taskinfo.h"
#include "circulartimerwidget.h"
#include "appusagetracker.h"
#include "appusageitem.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>
#include <windows.h>
#include <shellapi.h>
#include <QPixmap>
#include <QIcon>

static QString formatTime(qint64 ms);
QPixmap hiconToPixmap(HICON hIcon, int size = 32);

class FocusModeWnd : public QWidget
{
    Q_OBJECT
public:
    FocusModeWnd(QWidget* parent = nullptr);

    void initDatabase();

    void showTaskDialog();

signals:
    void switchToIndex();
    void switchToCalendar();
    void switchToProfile();

private:
    AppUsageTracker* tracker;

    NavigationBar* navBar;

    CircularTimerWidget* timerWidget;

    AddTask* addTaskWnd = nullptr;

    DatabaseManager* dataBase;

    QVector<TaskUI*> tasks;
    QVector<TaskUI*> completedTasks;

    QVBoxLayout* mainLayout;
    QVBoxLayout* tasksLayout;
    QVBoxLayout* completeTaskslayout;

    QLabel* wndTitle;
    QLabel* applications;

    QPushButton* startStopFocusBtn;

    QListWidget *appList;

    QTimer* uiUpdateTimer;

    bool focusRunning;

private slots:
    void startStopFocus();
    void updateAppUsageList();

};

#endif // FOCUSMODEWND_H
