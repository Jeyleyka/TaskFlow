#ifndef WARNINGWND_H
#define WARNINGWND_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDialog>
#include <QApplication>
#include <QScreen>
#include <QPropertyAnimation>

#include "thememanager.h"

class WarningWnd : public QDialog
{
    Q_OBJECT

public:
    explicit WarningWnd(const QString& warningText, QWidget* parent = nullptr);

    void showWithAnimation();

private:
    QWidget* container;

    QVBoxLayout* mainLayout;
    QVBoxLayout* containerLayout;

    QLabel* warningText;

    QPushButton* closeBtn;

    QPropertyAnimation* animation;
};

#endif // WARNINGWND_H
