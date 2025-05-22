#ifndef PRIORITYITEMWIDGET_H
#define PRIORITYITEMWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>

class PriorityItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PriorityItemWidget(const int priori, QWidget* parent = nullptr);

    int getPriority() const;

signals:
    void clickedWithId(int priority);
    void openPriorityWnd();

private:
    QVBoxLayout* mainLayout;

    QLabel* priority;

    QToolButton* iconBtn;

    int priorityInt;
};

#endif // PRIORITYITEMWIDGET_H
