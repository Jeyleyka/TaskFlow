#ifndef INFOFORNEWUSERWND_H
#define INFOFORNEWUSERWND_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QDialog>
#include <QVBoxLayout>
#include <QList>

#include "intro.h"

class InfoForNewUserWnd : public QDialog
{
    Q_OBJECT

public:
    InfoForNewUserWnd(QWidget *parent = nullptr);

    // void createSlideButtons();
    void createSlide(QWidget*& slide, QString iconName, QSize size, QString titleText, QString descText, int index);

// private slots:
//     void updateSlideButtons(int activeIndex);

private:
    QPushButton* skipSlides;
    QPushButton* backSlide;
    QPushButton* nextSlide;
    QPushButton* iconBtn;
    QPushButton* activeBtn = nullptr;

    QWidget* manageSlide;
    QWidget* dailySlide;
    QWidget* orgonaizeSlide;

    QLabel* wndTitle;
    QLabel* description;

    QStackedWidget* stackedWidget;

    QVBoxLayout* mainLayout;

    QHBoxLayout* btnsLayout;

    QList<QPushButton*> pageBtns;
};

#endif // INFOFORNEWUSERWND_H
