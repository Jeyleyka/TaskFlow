#ifndef INTRO_H
#define INTRO_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "login.h"
#include "createaccount.h"

class Intro : public QDialog
{
    Q_OBJECT

public:
    Intro(QWidget* parent = nullptr);
    ~Intro();

private:
    QWidget* container;

    QVBoxLayout* mainLayout;

    QHBoxLayout* btnsLayout;

    QPushButton* prevBtn;
    QPushButton* closeBtn;
    QPushButton* loginBtn;
    QPushButton* createAccBtn;

    QLabel* wndTitle;
    QLabel* subtitle;
};

#endif // INTRO_H
