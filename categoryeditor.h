#ifndef CATEGORYEDITOR_H
#define CATEGORYEDITOR_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QColorDialog>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QPainter>
#include <Qcolor>

class CategoryEditor : public QDialog
{
    Q_OBJECT
public:
    explicit CategoryEditor(QWidget* parent = nullptr);
    QString getName() const;
    QString getColor() const;
    QString getIcon() const;

signals:
    void categoryCreated(const QString& name, const QColor& color, const QIcon& icon);

private:
    QLabel* wndTitle;
    QLabel* categoryName;
    QLabel* categoryIcon;

    QLineEdit* nameEdit;

    QPushButton* colorButton;
    QPushButton* cancel;
    QPushButton* createCategory;

    QComboBox* iconCombo;

    QPushButton* saveButton;

    QString selectedColor;

    QVBoxLayout* mainLayout;

    QHBoxLayout* btnsLayout;

private slots:
    void onCreateCategoryClicked();
    void chooseColor();
    void saveCategory();
};

#endif // CATEGORYEDITOR_H
