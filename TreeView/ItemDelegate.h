#pragma once

#include <QStyledItemDelegate>

class TreeViewModel;

class ItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ItemDelegate(TreeViewModel *model, QObject *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    bool editorEvent(QEvent *event,
                     QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index) override;

signals:
    void buttonClicked(const QModelIndex &index);
    void needUpdate(const QModelIndex &index);

private:
    QRect calculateIconRect(const QStyleOptionViewItem &option) const;
    QRect calculateTextRect(const QStyleOptionViewItem &option) const;
    QRect calculateButtonRect(const QRect &textRect,
                              const QString &itemText,
                              QFontMetrics fm) const;

    TreeViewModel *mModel = nullptr;
};
