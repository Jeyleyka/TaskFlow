#ifndef ABOUTUSWND_H
#define ABOUTUSWND_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QDialog>
#include <QVBoxLayout>

#include "thememanager.h"

class AboutUsWnd : public QDialog
{
    Q_OBJECT

public:
    AboutUsWnd(QWidget *parent = nullptr);

private:
    QVBoxLayout* mainLayout;

    QPushButton* closeBtn;

    QLabel* wndTitle;
    QLabel* intro;
    QLabel* middle;
    QLabel* gratitude;

    QFrame* line;
};

#endif // ABOUTUSWND_H
