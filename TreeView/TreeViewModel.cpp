#include "TreeViewModel.h"

#include "TreeViewItem.h"

TreeViewModel::TreeViewModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    buildModel();
}

TreeViewModel::~TreeViewModel() = default;

QModelIndex TreeViewModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    TreeViewItem *item = nullptr;

    if (parent.isValid()) {
        const TreeViewItem *parentItem = static_cast<TreeViewItem *>(parent.internalPointer());
        item = parentItem->child(row);
    } else {
        if (row < 0 || row >= mTopItems.size()) {
            return QModelIndex();
        }

        item = mTopItems[row].get();
    }

    return createIndex(row, column, item);
}

QModelIndex TreeViewModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    const TreeViewItem *item = static_cast<TreeViewItem *>(index.internalPointer());

    if (TreeViewItem *parentItem = item->parentItem()) {
        return createIndex(parentItem->row(), 0, parentItem);
    }

    return QModelIndex();
}

int TreeViewModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        return mTopItems.size();
    }

    TreeViewItem *parentItem = static_cast<TreeViewItem *>(parent.internalPointer());
    return parentItem->childCount();
}

int TreeViewModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant TreeViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    TreeViewItem *item = static_cast<TreeViewItem *>(index.internalPointer());
    return item->data();
}

void TreeViewModel::buildModel()
{
    for (uint32_t i = 0; i < 5; ++i) {
        auto topItem = std::make_unique<TreeViewItem>(QString::number(i), nullptr);

        for (uint32_t j = 0; j < 3; ++j) {
            auto childItem = std::make_unique<TreeViewItem>(QString::number(j), topItem.get());
            topItem->appendChild(std::move(childItem));
        }

        mTopItems.push_back(std::move(topItem));
    }
}
