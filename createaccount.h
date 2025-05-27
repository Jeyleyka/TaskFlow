#ifndef CREATEACCOUNT_H
#define CREATEACCOUNT_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QBuffer>

#include "intro.h"
#include "mainwindow.h"

class CreateAccount : public QDialog
{
    Q_OBJECT

public:
    CreateAccount(QWidget* parent = nullptr);
    ~CreateAccount();

private:
    QLabel* wndTitle;
    QLabel* usernameLabel;
    QLabel* passwordLabel;
    QLabel* confirmPassLabel;

    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QLineEdit* confirmPassEdit;

    QPushButton* prevWndBtn;
    QPushButton* closeWndBtn;
    QPushButton* loginBtn;
    QPushButton* haventAccount;

    QHBoxLayout* btnsLayout;

    QVBoxLayout* usernameLayout;
    QVBoxLayout* passwordLayout;
    QVBoxLayout* confirmPassLayout;
    QVBoxLayout* mainLayout;

private slots:
    bool createAccToDB(const QString &name, const QString &password);
};

#endif // CREATEACCOUNT_H
