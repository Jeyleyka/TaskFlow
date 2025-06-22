#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include "task.h"
#include "choosecategory.h"
#include "categoryitemwidget.h"
#include "choosepriority.h"
#include "priorityitemwidget.h"
#include "customcalendar.h"

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QCloseEvent>
#include <QPushButton>

class AddTask : public QWidget
{
    Q_OBJECT

public:
    explicit AddTask(QWidget* parent = nullptr);
    ~AddTask();

    void showCentered(QWidget* parent);

    Task getTask() const;
    CategoryItemWidget* getSelectedCategoryWidget();

signals:
    void categorySelectedWidget(CategoryItemWidget* widget);
    void onCreateTask();
    void onCloseWnd();

protected:
    void focusOutEvent(QFocusEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void enterEditMode(QLabel* label, QLineEdit* editor);
    void leaveEditMode(QLabel* label, QLineEdit* editor);

private:
    ChooseCategory* chooseCategoryWnd;

    ChoosePriority* choosePriorityWnd;

    CategoryItemWidget* itemWidget;

    PriorityItemWidget* priorityWidget;

    QWidget* container;

    QLineEdit* titleLineEdit;
    QLineEdit* descLineEdit;

    QComboBox* priorityComboBox;
    QComboBox* statusComboBox;

    QPushButton* chooseCategory;
    QPushButton* choosePriority;
    QPushButton* chooseDate;
    QPushButton* addTaskBtn;
    QPushButton* closeWndBtn;

    QDate date;

    QVBoxLayout* containerLayout;
    QVBoxLayout* mainLayout;

    QHBoxLayout* buttonsLayout;

    QLabel* wndTitle;
    QLabel* taskTitle;
    QLabel* description;

    int hour;
    int minutes;

    bool isAm;
    bool isClosing;
};

#endif // TASKDIALOG_H
