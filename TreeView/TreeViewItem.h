#pragma once

#include <QString>
#include <vector>

class TreeViewItem
{
public:
    TreeViewItem();
    TreeViewItem(QString text, TreeViewItem *parentItem);

    void appendChild(std::unique_ptr<TreeViewItem> child);

    QString data() const;
    TreeViewItem *parentItem() const;
    TreeViewItem *child(int row) const;
    int childCount() const;
    int row() const;

private:
    QString mText;
    TreeViewItem *mParentItem = nullptr;
    std::vector<std::unique_ptr<TreeViewItem>> mChildren;
};
