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
#include <QSqlQuery>
#include <QSqlError>

#include "categoryitemwidget.h"
#include "UserSession.h"
#include "thememanager.h"

class TaskUI : public QWidget {
    Q_OBJECT

public:
    TaskUI(QString titleStr, QString desc, QString createData, int priority,
           QString categoryName, QColor categoryColor, QIcon categoryIcon, int id, int completed,
           QWidget* parent = nullptr);

    TaskUI();

    void setTitle(QString& newTitle);
    void setDesc(QString& newDesc);
    void setCategory(QString name, QColor color, QIcon icon, int width, int height);
    void setPriority(QString priority) const;
    void setCompleted(bool completed);

    int getId() const;
    QString getTitle() const;
    QString getCategoryName() const;
    QColor getCategoryColor() const;
    QIcon getCategoryIcon() const;
    QString getDate() const;
    int getPriority() const;
    bool getCompleted() const;

signals:
    void taskClicked();
    void onUpdateTaskToComplete(const int taskId, bool completed);
    void taskCreatedInIndex(const QString name, const QString desc, const QString dueDate, const int priority, const QString catName, QColor catColor,
                            QIcon catIco, const int id, const int completed);
    void taskCreatedInCalendar(const QString name, const QString desc, const QString dueDate, const int priority, const QString catName, QColor catColor,
                               QIcon catIco, const int id, const int completed);

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    QLabel* title;
    QLabel* description;
    QLabel* creationDate;
    QLabel* categoryLabel;

    QIcon categoryIcon;

    QString createDate;

    QColor categoryColor;

    QPushButton* showDescription;
    QPushButton* circle;
    QPushButton* categoryBtn;

    int counter;
    int priority;
    int taskID;

    bool completed;

    void updateData();
};

#endif // TASKUI_H
