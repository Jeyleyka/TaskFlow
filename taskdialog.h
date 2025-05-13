#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include "task.h"

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

class TaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskDialog(QWidget* parent = nullptr);
    ~TaskDialog();

    Task getTask() const;

public slots:
    void hideWithAnim();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    QLineEdit* titleLineEdit;

    QTextEdit* descriptionTextEdit;

    QDateEdit* dueDateEdit;

    QComboBox* priorityComboBox;
    QComboBox* statusComboBox;

    QDialogButtonBox* buttonBox;

    QFormLayout* formLayout;

    QVBoxLayout* mainLayout;

    bool isClosing;
};

#endif // TASKDIALOG_H
