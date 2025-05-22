#ifndef TASKUI_H
#define TASKUI_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QMouseEvent>
#include <QBuffer>

#include "categoryitemwidget.h"

class TaskUI : public QWidget {
    Q_OBJECT

public:
    TaskUI(QString titleStr, QString desc, QString createData, int priority,
           QString categoryName, QColor categoryColor, QIcon categoryIcon,
           QWidget* parent = nullptr);

    void setTitle(QString& newTitle);
    void setDesc(QString& newDesc);
    void setCategory(QString name, QColor color, QIcon icon, int width, int height);
    void setPriority(QString priority) const;

    QString getCategoryName() const;
    QColor getCategoryColor() const;
    QIcon getCategoryIcon() const;

    int getPriority() const;

signals:
    void taskClicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    QLabel* title;
    QLabel* description;
    QLabel* creationDate;
    QLabel* categoryLabel;

    QIcon categoryIcon;

    QColor categoryColor;

    QPushButton* showDescription;
    QPushButton* circle;
    QPushButton* categoryBtn;

    int counter;
    int priority;
};

#endif // TASKUI_H
