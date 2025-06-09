#ifndef CHANGEACCOUNTNAMEWND_H
#define CHANGEACCOUNTNAMEWND_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

#include "UserSession.h"
#include "thememanager.h"
#include "warningwnd.h"

class ChangeAccountNameWnd : public QDialog
{
    Q_OBJECT

public:
    ChangeAccountNameWnd(QWidget *parent = nullptr);
    ~ChangeAccountNameWnd();

signals:
    void onUpdateName();

private:
    QWidget* container;

    QLabel* wndTitle;

    QFrame* line;

    QLineEdit* newUsername;

    QPushButton* cancel;
    QPushButton* edit;

    QVBoxLayout* mainLayout;

    QHBoxLayout* btnsLayout;

    QString oldName;

private slots:
    void changeNameInDB();
};

#endif // CHANGEACCOUNTNAMEWND_H
