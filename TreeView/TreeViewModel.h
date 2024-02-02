#pragma once

#include <QAbstractItemModel>

class TreeViewItem;
class Database;

class TreeViewModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeViewModel(const QString &dbPath, QObject *parent = nullptr);
    ~TreeViewModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    void buildModel();

    Database *mDatabase = nullptr;
    std::vector<std::unique_ptr<TreeViewItem>> mTopItems;
};
