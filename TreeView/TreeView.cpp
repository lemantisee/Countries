#include "TreeView.h"

#include "TreeViewModel.h"

TreeView::TreeView(const QString &dbPath, QWidget *parent)
    : QTreeView(parent)
{
    setHeaderHidden(true);
    setModel(new TreeViewModel(dbPath));
}
