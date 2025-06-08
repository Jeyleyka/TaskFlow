#ifndef CHANGEPASSWND_H
#define CHANGEPASSWND_H

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

class ChangePass : public QDialog
{
    Q_OBJECT

public:
    ChangePass(QWidget *parent = nullptr);
    ~ChangePass();

private:
    QWidget* container;

    QLabel* wndTitle;
    QLabel* oldPassword;
    QLabel* newPassword;

    QFrame* line;

    QLineEdit* oldPasswordEdit;
    QLineEdit* newPasswordEdit;

    QPushButton* cancel;
    QPushButton* edit;

    QVBoxLayout* mainLayout;

    QHBoxLayout* btnsLayout;

    QString oldPass;

private slots:
    void changePassInDB();
};

#endif // CHANGEPASSWND_H
