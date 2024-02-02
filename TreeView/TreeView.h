#pragma once

#include <QTreeView>

class TreeView : public QTreeView
{
    Q_OBJECT
public:
    TreeView(const QString &dbPath, QWidget *parent = nullptr);
};
