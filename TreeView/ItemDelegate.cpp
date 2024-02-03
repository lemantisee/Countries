#include "ItemDelegate.h"

#include <QPainter>
#include <QDebug>
#include <QApplication>

ItemDelegate::ItemDelegate(QObject *parent)
    :QStyledItemDelegate(parent)
{}

void ItemDelegate::paint(QPainter *painter,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    painter->save();

    QStyle *style = option.widget ? option.widget->style() : QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, option.widget);

    const QIcon icon = index.model()->data(index, Qt::DecorationRole).value<QIcon>();
    QRect iconRect(0, 0, 16, 16);
    iconRect.moveCenter(option.rect.center());
    iconRect.moveLeft(option.rect.left());

    painter->drawPixmap(iconRect, icon.pixmap({16, 16}));

    QRect textRect = QRect(0, 0, option.rect.width() - iconRect.width(), option.rect.height());

    textRect.moveBottomLeft(option.rect.bottomLeft());
    textRect.translate(iconRect.width() + 5, 0);

    if (!index.parent().isValid()) {
        QFont font = painter->font();
        font.setBold(true);
        painter->setFont(font);
    }

    QString text = index.model()->data(index, Qt::DisplayRole).toString();
    painter->drawText(textRect, text);

    if (option.state.testFlag(QStyle::State_HasFocus)) {
        if (const QStyleOptionViewItem *vopt = qstyleoption_cast<const QStyleOptionViewItem *>(
                &option)) {
            style->proxy()->subElementRect(QStyle::SE_ItemViewItemFocusRect, vopt, option.widget);

            QStyleOptionFocusRect o;
            o.QStyleOption::operator=(*vopt);
            o.rect = style->subElementRect(QStyle::SE_ItemViewItemFocusRect, vopt, option.widget);
            o.state |= QStyle::State_KeyboardFocusChange;
            o.state |= QStyle::State_Item;
            QPalette::ColorGroup cg = (vopt->state & QStyle::State_Enabled) ? QPalette::Normal
                                                                            : QPalette::Disabled;
            o.backgroundColor = vopt->palette.color(cg,
                                                    (vopt->state & QStyle::State_Selected)
                                                        ? QPalette::Highlight
                                                        : QPalette::Window);
            style->drawPrimitive(QStyle::PE_FrameFocusRect, &o, painter, option.widget);
        }
    }

    painter->restore();
}
