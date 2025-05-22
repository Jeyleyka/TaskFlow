#ifndef EDITTASK_H
#define EDITTASK_H

#include "categoryitemwidget.h"
#include "priorityitemwidget.h"
#include "choosecategory.h"
#include "choosepriority.h"

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QString>
#include <QEvent>
#include <QPushButton>
#include <QKeyEvent>
#include <QFrame>
#include <QComboBox>

class EditTask : public QDialog
{
    Q_OBJECT

public:
    explicit EditTask(QString titleStr, QString descStr, QString category_name, QColor category_color, QIcon category_icon, int priori, QWidget* parent = nullptr);

    QString text() const;
    QString getDescription() const;
    QString getPriority() const;
    QString getCategoryName() const;
    QColor getCategoryColor() const;
    QIcon getCategoryIcon() const;
    void setText(const QString& text);

signals:
    void textEdited(const QString& newText);
    void updateEdit();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void enterEditMode(QLabel* label, QLineEdit* editor);
    void leaveEditMode(QLabel* label, QLineEdit* editor);
    void onUpdateData();

private:
    ChoosePriority* priorityWnd;
    ChooseCategory* categoryWnd;
    CategoryItemWidget* categoryWidget;
    PriorityItemWidget* priorityWidget;

    QLabel* wndTitle;
    QLabel* title;
    QLabel* description;
    QLabel* priority;

    QString categoryName;

    QColor categoryColor;

    QIcon categoryIcon;

    QFrame* line;

    QLineEdit* editTitle;
    QLineEdit* editDesc;

    QVBoxLayout* mainLayout;
    QHBoxLayout* btnsLayout;

    QPushButton* cancel;
    QPushButton* edit;
};

#endif // EDITTASK_H
