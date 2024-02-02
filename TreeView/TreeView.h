#pragma once

#include <QTreeView>

class TreeViewModel;
class Database;

class TreeView : public QTreeView
{
    Q_OBJECT
public:
    TreeView(const QString &dbPath, QWidget *parent = nullptr);

private:
    void onDoubleClick(const QModelIndex &index);
    void onBeforeModelUpdate();
    void onModelUpdated();

    TreeViewModel *mModel = nullptr;
    Database *mDatabase = nullptr;
    std::vector<QString> mExpandedItems;
};
