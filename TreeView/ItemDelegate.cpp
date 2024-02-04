#include "ItemDelegate.h"

#include <QPainter>
#include <QApplication>
#include <QMouseEvent>

#include "TreeViewModel.h"

ItemDelegate::ItemDelegate(TreeViewModel *model, QObject *parent)
    : QStyledItemDelegate(parent), mModel(model)
{
}

void ItemDelegate::paint(QPainter *painter,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    painter->save();

    QStyle *style = option.widget ? option.widget->style() : QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, option.widget);

    const QRect iconRect = calculateIconRect(option);
    const QIcon icon = index.model()->data(index, Qt::DecorationRole).value<QIcon>();
    painter->drawPixmap(iconRect, icon.pixmap({16, 16}));

    if (!index.parent().isValid()) {
        QFont font = painter->font();
        font.setBold(true);
        painter->setFont(font);
    }

    const QRect textRect = calculateTextRect(option);
    const QString text = index.model()->data(index, Qt::DisplayRole).toString();
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

    if (option.state.testFlag(QStyle::State_MouseOver) && index.parent().isValid()) {
        QStyleOptionButton opt;
        opt.state |= QStyle::State(index.data(TreeViewModel::ButtonState).toInt());
        opt.state |= QStyle::State_Enabled;
        opt.rect = calculateButtonRect(textRect, text, painter->fontMetrics());
        opt.text = "+";
        style->drawControl(QStyle::CE_PushButton, &opt, painter, 0);
        mModel->setData(index, opt.rect, TreeViewModel::ButtonGeometry);
    }

    painter->restore();
}

bool ItemDelegate::editorEvent(QEvent *event,
                               QAbstractItemModel *model,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        QRect buttonGeo = model->data(index, TreeViewModel::ButtonGeometry).toRect();
        if (buttonGeo.contains(mouseEvent->pos())) {
            model->setData(index,
                           int(QStyle::State_Sunken | QStyle::State_MouseOver),
                           TreeViewModel::ButtonState);
            emit buttonClicked(index);
            emit needUpdate(index);
        }
    }

    if (event->type() == QEvent::MouseButtonRelease) {
        model->setData(index,
                       int(QStyle::State_Enabled | QStyle::State_MouseOver),
                       TreeViewModel::ButtonState);
        emit needUpdate(index);
    }

    if (event->type() == QEvent::MouseMove) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        QRect buttonGeo = model->data(index, TreeViewModel::ButtonGeometry).toRect();
        if (buttonGeo.contains(mouseEvent->pos())) {
            model->setData(index, QStyle::State_MouseOver, TreeViewModel::ButtonState);
        } else {
            model->setData(index, QStyle::State_Enabled, TreeViewModel::ButtonState);
        }

        emit needUpdate(index);
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QRect ItemDelegate::calculateIconRect(const QStyleOptionViewItem &option) const
{
    QRect iconRect(0, 0, 16, 16);
    iconRect.moveCenter(option.rect.center());
    iconRect.moveLeft(option.rect.left());
    return iconRect;
}

QRect ItemDelegate::calculateTextRect(const QStyleOptionViewItem &option) const
{
    QRect iconRect = calculateIconRect(option);
    QRect textRect = QRect(0, 0, option.rect.width() - iconRect.width(), option.rect.height());

    textRect.moveBottomLeft(option.rect.bottomLeft());
    textRect.translate(iconRect.width() + 5, 0);

    return textRect;
}

QRect ItemDelegate::calculateButtonRect(const QRect &textRect,
                                        const QString &itemText,
                                        QFontMetrics fm) const
{
    int textWidth = fm.boundingRect(itemText).width();

    QPoint buttonPos = textRect.bottomLeft();
    buttonPos.rx() += textWidth + 5;
    QRect buttonRect(0, 0, 16, 16);
    buttonRect.moveCenter(textRect.center());
    buttonRect.moveLeft(buttonPos.x());

    return buttonRect;
}
