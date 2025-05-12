#ifndef DELETEBUTTONDELEGATE_H
#define DELETEBUTTONDELEGATE_H

#include <QStyledItemDelegate>
#include <QPushButton>
#include <QApplication>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QObject>
#include <QMouseEvent>

class DeleteButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DeleteButtonDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void deleteRowRequested(const int row) const;
};

#endif // DELETEBUTTONDELEGATE_H
