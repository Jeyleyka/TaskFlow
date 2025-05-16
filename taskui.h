#ifndef TASKUI_H
#define TASKUI_H

#include <QWidget>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QString>

class TaskUI : public QWidget
{
    Q_OBJECT
public:
    explicit TaskUI(QString titleStr, QString desc, QString createData, QString priorityStr, QWidget* parent = nullptr);

    void mousePressEvent(QMouseEvent *event) override;
private:
    QLabel* title;
    QLabel* creationDate;
    QLabel* description;

    QPushButton* showDescription;
    QPushButton* successTask;

signals:
    void taskClicked();
    // QHBoxLayout* mainLayout;
};

#endif // TASKUI_H
