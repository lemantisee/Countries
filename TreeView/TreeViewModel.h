#pragma once

#include <QAbstractItemModel>

#include <optional>

#include "CountryRecord.h"

class TreeViewItem;
class Database;

class TreeViewModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeViewModel(Database *database, QObject *parent = nullptr);
    ~TreeViewModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool isChild(const QModelIndex &index) const;
    std::optional<Operator> getOperator(const QModelIndex &index) const;
    std::vector<QModelIndex> getTopItems() const;
    QString getCountryName(const QModelIndex &index) const;
    QModelIndex getTopLevelIndex(const QString &country) const;

private:
    void updateModel();

    Database *mDatabase = nullptr;
    std::vector<std::unique_ptr<TreeViewItem>> mTopItems;
};
