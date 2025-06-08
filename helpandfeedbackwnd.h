#ifndef HELPANDFEEDBACKWND_H
#define HELPANDFEEDBACKWND_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>

#include "thememanager.h"

class HelpAndFeedbackWnd : public QDialog
{
    Q_OBJECT

public:
    HelpAndFeedbackWnd(QWidget *parent = nullptr);

private:
    QVBoxLayout* mainLayout;

    QLabel* wndTitle;
    QLabel* git;
    QLabel* inst;
    QLabel* tg;
    QLabel* inform;

    QPushButton* closeBtn;

    QFrame* line;
};

#endif // HELPANDFEEDBACKWND_H
