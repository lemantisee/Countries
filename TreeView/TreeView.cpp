#include "TreeView.h"

#include "TreeViewModel.h"

TreeView::TreeView(QWidget *parent)
    :QTreeView(parent)
{
    setHeaderHidden(true);
    setModel(new TreeViewModel);
}
