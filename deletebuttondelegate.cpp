#include "deletebuttondelegate.h"

DeleteButtonDelegate::DeleteButtonDelegate(QObject* parent) : QStyledItemDelegate(parent) {

}

void DeleteButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionButton button;
    button.rect = option.rect;
    button.text = "❌";
    button.state = QStyle::State_Enabled;

    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
}

bool DeleteButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    if (event->type() == QEvent::MouseButtonRelease) {
        auto *mouseEvent = static_cast<QMouseEvent*>(event);

        if (option.rect.contains(mouseEvent->pos())) {
            emit deleteRowRequested(index.row());
            return true;
        }
    }

    return false;
}
