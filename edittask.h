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
    explicit EditTask(QString titleStr, QString descStr, QWidget* parent = nullptr);

    QString getTitle() const;
    QString getDescription() const;
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
    QLabel* wndTitle;
    QLabel* title;
    QLabel* description;

    QFrame* line;

    QLineEdit* editTitle;
    QLineEdit* editDesc;

    QVBoxLayout* mainLayout;
    QHBoxLayout* btnsLayout;

    QPushButton* cancel;
    QPushButton* edit;
};

#endif // EDITTASK_H
