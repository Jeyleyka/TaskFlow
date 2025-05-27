#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

#include "intro.h"
#include "mainwindow.h"

class Login : public QDialog
{
    Q_OBJECT

public:
    Login(QWidget* parent = nullptr);
    ~Login();

private:
    QLabel* wndTitle;
    QLabel* usernameLabel;
    QLabel* passwordLabel;

    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;

    QPushButton* prevWndBtn;
    QPushButton* closeWndBtn;
    QPushButton* loginBtn;
    QPushButton* haventAccount;

    QHBoxLayout* btnsLayout;

    QVBoxLayout* usernameLayout;
    QVBoxLayout* passwordLayout;
    QVBoxLayout* mainLayout;

private slots:
    bool CheckDataToCorrectly(const QString& name, const QString& password);
};

#endif // LOGIN_H
