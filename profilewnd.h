#ifndef PROFILEWND_H
#define PROFILEWND_H

#include <QWidget>
#include <QLabel>
#include <QIcon>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlError>

#include "navigationbar.h"
#include "addtask.h"
#include "taskui.h"
#include "taskinfo.h"
#include "database.h"
#include "changeaccountnamewnd.h"
#include "changepasswnd.h"
#include "changeimgwnd.h"
#include "UserSession.h"
#include "aboutuswnd.h"
#include "helpandfeedbackwnd.h"
#include "profilesettingswidget.h"
#include "themedialog.h"
#include "changelanguagewnd.h"

class ProfileWnd : public QWidget
{
    Q_OBJECT
public:
    ProfileWnd(QWidget *parent = nullptr);
    ~ProfileWnd();

public slots:
    void initTasksStatus();
    void updateTasksData();

signals:
    void switchToIndex();
    void switchToCalendar();
    void switchToFocus();
    void onChangeImg(QPixmap pixmap);
    void onLogOut();

private:
    NavigationBar* navBar;

    AddTask* addTaskWnd = nullptr;

    DatabaseManager* dataBase;

    ChangeImgWnd* changeImgWnd;

    AboutUsWnd* about;

    HelpAndFeedbackWnd* feedback;

    QLabel* wndTitle;
    QLabel* username;
    QLabel* settings;
    QLabel* account;
    QLabel* taskFlow;
    QLabel* tasksLeft;
    QLabel* tasksDone;
    QLabel* iconLabel;

    ProfileSettingsWidget* changeAppColor;
    ProfileSettingsWidget* changeAppLanguage;
    ProfileSettingsWidget* appSettingsBtn;
    ProfileSettingsWidget* changeAccNameBtn;
    ProfileSettingsWidget* changePasswBtn;
    ProfileSettingsWidget* changeAccImg;
    ProfileSettingsWidget* aboutUsBtn;
    ProfileSettingsWidget* helpAndFeedBtn;
    ProfileSettingsWidget* supportBtn;

    QPushButton* logOutBtn;

    QVector<TaskUI*> tasks;
    QVector<TaskUI*> completedTasks;

    QVBoxLayout* mainLayout;
    QVBoxLayout* tasksLayout;
    QVBoxLayout* completeTaskslayout;

    QHBoxLayout* tasksLeftAndDoneLayout;

    int countOfCompleted = 0;

private slots:
    // void showTaskDialog();
};

#endif // PROFILEWND_H
