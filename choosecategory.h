#ifndef CHOOSECATEGORY_H
#define CHOOSECATEGORY_H

#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QList>
#include <QListWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QByteArray>
#include <QBuffer>
#include <QMessageBox>

#include "categoryeditor.h"
#include "categoryitemwidget.h"

class ChooseCategory : public QDialog
{
    Q_OBJECT
public:
    explicit ChooseCategory(QWidget* parent = nullptr);

signals:
    void categorySelected(const QString& name, const QColor& color, const QIcon& icon);
    void AddCategorySignal();

public slots:
    void addCategory(const QString& name, const QColor& color, const QIcon& icon);
    void addCategoryToUI(const QString &name, const QColor &color, const QIcon &icon);

private:
    CategoryEditor* editor;

    QLabel* wndTitle;

    QFrame* line;

    QPushButton* addCategoryBtn;

    QVBoxLayout* mainLayout;

    QList<CategoryEditor*> categories;

    QListWidget* listWidget;

private slots:
    void onAddCategory();
};

#endif // CHOOSECATEGORY_H
