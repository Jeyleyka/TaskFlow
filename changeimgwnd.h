#ifndef CHANGEIMGWND_H
#define CHANGEIMGWND_H

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
#include <QFileDialog>
#include <QBuffer>

#include "UserSession.h"

class ChangeImgWnd : public QDialog
{
    Q_OBJECT

public:
    ChangeImgWnd(QWidget *parent = nullptr);
    ~ChangeImgWnd();

signals:
    void onUpdateImg();

private:
    QWidget* container;

    QLabel* wndTitle;

    QFrame* line;

    QPushButton* importBtn;
    QPushButton* closeBtn;

    QVBoxLayout* mainLayout;

    QIcon selectedIcon;

private slots:
    void changeImgInDB();
};

#endif // CHANGEIMGWND_H
