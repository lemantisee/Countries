#pragma once

#include <QString>
#include <QIcon>
#include <QStyle>

#include "CountryRecord.h"

class TreeViewItem
{
public:
    TreeViewItem();
    TreeViewItem(QString countryName, QString countryCode, TreeViewItem *parentItem);
    TreeViewItem(Operator op, TreeViewItem *parentItem);

    void appendChild(std::unique_ptr<TreeViewItem> child);
    void setButtonGeo(QRect rect);
    void setButtonState(QStyle::State state);

    QString data() const;
    TreeViewItem *parentItem() const;
    TreeViewItem *child(int row) const;
    int childCount() const;
    int row() const;
    std::optional<Operator> getOperator() const;
    QIcon getIcon() const;
    QRect getButtonGeo() const;
    QStyle::State getButtonState() const;

private:
    QString mText;
    QString mCountryCode;
    TreeViewItem *mParentItem = nullptr;
    std::vector<std::unique_ptr<TreeViewItem>> mChildren;
    std::optional<Operator> mOperator;
    QRect mButtonGeo;
    QStyle::State mButtonState = QStyle::State_Enabled;
};
