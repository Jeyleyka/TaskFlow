#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include "task.h"
#include "choosecategory.h"
#include "categoryitemwidget.h"

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

class TaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskDialog(QWidget* parent = nullptr);
    ~TaskDialog();

    Task getTask() const;
    CategoryItemWidget* getSelectedCategoryWidget();

signals:
    void categorySelectedWidget(CategoryItemWidget* widget);

public slots:
    void hideWithAnim();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    ChooseCategory* chooseCategoryWnd;
    CategoryItemWidget* itemWidget;

    QLineEdit* titleLineEdit;

    QTextEdit* descriptionTextEdit;

    QDateEdit* dueDateEdit;

    QComboBox* priorityComboBox;
    QComboBox* statusComboBox;

    QPushButton* chooseCategory;

    QDialogButtonBox* buttonBox;

    QFormLayout* formLayout;

    QVBoxLayout* mainLayout;

    bool isClosing;
};

#endif // TASKDIALOG_H
