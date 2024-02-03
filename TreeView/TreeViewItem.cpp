#include "TreeViewItem.h"

#include "Icons.h"

TreeViewItem::TreeViewItem() {}

TreeViewItem::TreeViewItem(QString countryName, QString countryCode, TreeViewItem *parentItem)
    : mText(std::move(countryName))
    , mCountryCode(std::move(countryCode))
    , mParentItem(parentItem)
{

}

TreeViewItem::TreeViewItem(Operator op, TreeViewItem *parentItem)
    : mParentItem(parentItem)
    , mOperator(std::move(op))
{
    mText = QString("%1 (%2, %3)").arg(mOperator->name).arg(mOperator->mcc).arg(mOperator->mnc);
}

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

std::optional<Operator> TreeViewItem::getOperator() const
{
    return mOperator;
}

QIcon TreeViewItem::getIcon() const
{
    if (mOperator) {
        return Icons::getOperatorIcon(mOperator->mcc, mOperator->mnc);
    }

    return Icons::getCountryIcon(mCountryCode);
}
