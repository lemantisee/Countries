#include "TreeViewModel.h"

#include <QCollator>

#include "TreeViewItem.h"
#include "Database.h"

TreeViewModel::TreeViewModel(Database *database, QObject *parent)
    : QAbstractItemModel(parent)
    , mDatabase(database)
{
    updateModel();

    connect(mDatabase, &Database::updated, this, &TreeViewModel::updateModel);
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

bool TreeViewModel::isChild(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return false;
    }

    const TreeViewItem *item = static_cast<TreeViewItem *>(index.internalPointer());
    return item->parentItem() != nullptr;
}

std::optional<Operator> TreeViewModel::getOperator(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return std::nullopt;
    }

    const TreeViewItem *item = static_cast<TreeViewItem *>(index.internalPointer());
    return item->getOperator();
}

std::vector<QModelIndex> TreeViewModel::getTopItems() const
{
    std::vector<QModelIndex> list;
    for (int r = 0; r < mTopItems.size(); ++r) {
        const TreeViewItem *item = mTopItems[r].get();
        list.push_back(createIndex(r, 0, item));
    }

    return list;
}

QString TreeViewModel::getCountryName(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return {};
    }

    const TreeViewItem *item = static_cast<TreeViewItem *>(index.internalPointer());
    if (item->parentItem()) {
        return {};
    }

    return item->data();
}

QModelIndex TreeViewModel::getTopLevelIndex(const QString &country) const
{
    for (int r = 0; r < mTopItems.size(); ++r) {
        const TreeViewItem *item = mTopItems[r].get();
        if(item->data() == country) {
            return createIndex(r, 0, item);
        }
    }

    return {};
}

void TreeViewModel::updateModel()
{
    beginResetModel();

    mTopItems.clear();

    std::vector<CountryRecord> countries = mDatabase->getCountries();
    std::sort(countries.begin(),
              countries.end(),
              [](const CountryRecord &lvl, const CountryRecord &rvl) {
                  QCollator order;
                  return order.compare(lvl.name, rvl.name) < 0;
              });

    for (const CountryRecord &country: countries) {
        auto topItem = std::make_unique<TreeViewItem>(country.name, nullptr);

        for (const Operator &op: country.operators) {
            auto childItem = std::make_unique<TreeViewItem>(op, topItem.get());
            topItem->appendChild(std::move(childItem));
        }

        mTopItems.push_back(std::move(topItem));
    }

    endResetModel();
}
