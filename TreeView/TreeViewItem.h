#pragma once

#include <QString>
#include <QIcon>

#include "CountryRecord.h"

class TreeViewItem
{
public:
    TreeViewItem();
    TreeViewItem(QString countryName, QString countryCode, TreeViewItem *parentItem);
    TreeViewItem(Operator op, TreeViewItem *parentItem);

    void appendChild(std::unique_ptr<TreeViewItem> child);

    QString data() const;
    TreeViewItem *parentItem() const;
    TreeViewItem *child(int row) const;
    int childCount() const;
    int row() const;
    std::optional<Operator> getOperator() const;
    QIcon getIcon() const;

private:
    QString mText;
    QString mCountryCode;
    TreeViewItem *mParentItem = nullptr;
    std::vector<std::unique_ptr<TreeViewItem>> mChildren;
    std::optional<Operator> mOperator;
};
