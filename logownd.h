#ifndef LOGOWND_H
#define LOGOWND_H

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

#include "intro.h"
#include "database.h"
#include "infofornewuserwnd.h"

class LogoWnd : public QDialog
{
    Q_OBJECT

public:
    LogoWnd(QWidget *parent = nullptr);

    void showWithAnimation();

private:
    DatabaseManager* db;

    QPushButton* logoBtn;

    QVBoxLayout* mainLayout;
};

#endif // LOGOWND_H
