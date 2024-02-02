#include "TreeViewItem.h"

TreeViewItem::TreeViewItem() {}

TreeViewItem::TreeViewItem(QString text, TreeViewItem *parentItem)
    : mText(std::move(text))
    , mParentItem(parentItem)
{}

void TreeViewItem::appendChild(std::unique_ptr<TreeViewItem> child)
{
    mChildren.push_back(std::move(child));
}

QString TreeViewItem::data() const
{
    return mText;
}

TreeViewItem *TreeViewItem::parentItem() const
{
    return mParentItem;
}

TreeViewItem *TreeViewItem::child(int row) const
{
    if (row < 0 || row >= mChildren.size()) {
        return nullptr;
    }

    return mChildren[row].get();
}

int TreeViewItem::childCount() const
{
    return mChildren.size();
}

int TreeViewItem::row() const
{
    if (!mParentItem) {
        return 0;
    }

    auto it = std::find_if(mParentItem->mChildren.begin(),
                           mParentItem->mChildren.end(),
                           [this](const std::unique_ptr<TreeViewItem> &child) {
                               return child.get() == this;
                           });

    if(it == mParentItem->mChildren.end()) {
        return 0;
    }

    return std::distance(mParentItem->mChildren.begin(), it);
}
