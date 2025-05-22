#ifndef CATEGORYITEMWIDGET_H
#define CATEGORYITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QIcon>
#include <QMouseEvent>

class CategoryItemWidget : public QWidget
{
    Q_OBJECT

public:
    CategoryItemWidget(const QString& nameStr, const QColor& colorC, const QIcon& iconC, QWidget* parent = nullptr);

    QString getName() const;
    QColor getColor() const;
    QIcon getIcon() const;

signals:
    void itemClicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QLabel* name;

    QColor color;

    QIcon icon;
};

#endif // CATEGORYITEMWIDGET_H
