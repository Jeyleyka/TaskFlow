#ifndef CHOOSEPRIORITY_H
#define CHOOSEPRIORITY_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QList>

#include "flowlayout.h"
#include "priorityitemwidget.h"

class ChoosePriority : public QDialog
{
    Q_OBJECT

public:
    explicit ChoosePriority(QWidget* parent = nullptr);

    const int& getPrority();

signals:
    void prioritySelected(const int priority);

private:
    QLabel* titleWnd;

    QPushButton* cancel;
    QPushButton* save;

    QFrame* line;

    QVBoxLayout* mainLayout;
    QVBoxLayout* containerLayout;

    FlowLayout* widgetsLayout;
    QHBoxLayout* btnsLayout;


    QList<PriorityItemWidget*> widgets;

    int prior;
};

#endif // CHOOSEPRIORITY_H
