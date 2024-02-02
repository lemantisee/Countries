#pragma once

#include <QAbstractItemModel>

class TreeViewItem;

class TreeViewModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeViewModel(QObject *parent = nullptr);
    ~TreeViewModel();

    // Basic functionality:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    void buildModel();

    std::vector<std::unique_ptr<TreeViewItem>> mTopItems;
};
