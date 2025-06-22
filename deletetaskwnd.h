#ifndef DELETETASKWND_H
#define DELETETASKWND_H

#include <QWidget>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>


class DeleteTaskWnd : public QWidget
{
    Q_OBJECT

public:
    DeleteTaskWnd(QString taskTitle, int taskId, QWidget *parent = nullptr);

signals:
    void deleteTask();

private:

    QWidget* container;

    QLabel* wndTitle;
    QLabel* warning;
    QLabel* taskTitle;

    QFrame* line;

    QPushButton* cancelBtn;
    QPushButton* deleteBtn;

    QVBoxLayout* containerLayout;
    QVBoxLayout* mainLayout;

    QHBoxLayout* btnsLayout;
};

#endif // DELETETASKWND_H
