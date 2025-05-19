#ifndef CATEGORYITEMWIDGET_H
#define CATEGORYITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QIcon>

class CategoryItemWidget : public QWidget
{
    Q_OBJECT

public:
    CategoryItemWidget(const QString& nameStr, const QColor& colorC, const QIcon& iconC, QWidget* parent = nullptr);

    QString getName() const;
    QColor getColor() const;
    QIcon getIcon() const;
private:
    QLabel* name;

    QColor color;

    QIcon icon;
};

#endif // CATEGORYITEMWIDGET_H
