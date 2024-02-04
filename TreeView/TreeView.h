#pragma once

#include <QTreeView>

#include <QToolButton>

class TreeViewModel;
class Database;

class TreeView : public QTreeView
{
    Q_OBJECT
public:
    TreeView(const QString &dbPath, QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    void onDoubleClick(const QModelIndex &index);
    void onButtonClicked(const QModelIndex &index);
    void onAddClicked();
    void onBeforeModelUpdate();
    void onModelUpdated();
    void onOperatorClicked(int mcc, int mnc);

    TreeViewModel *mModel = nullptr;
    Database *mDatabase = nullptr;
    std::vector<QString> mExpandedItems;
    QToolButton *mAddButton = nullptr;
};
