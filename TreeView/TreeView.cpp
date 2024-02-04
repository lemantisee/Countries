#include "TreeView.h"

#include <QDebug>

#include "TreeViewModel.h"
#include "EditDialog.h"
#include "AddDialog.h"
#include "Database.h"
#include "ItemDelegate.h"

namespace {
constexpr uint32_t addButtonShift = 10;
}

TreeView::TreeView(const QString &dbPath, QWidget *parent)
    : QTreeView(parent)
    , mDatabase(new Database(dbPath, this))
{
    if (!mDatabase->open()) {
        return;
    }

    mModel = new TreeViewModel(mDatabase, this);
    mAddButton = new QToolButton(this);
    mAddButton->setFixedSize({30, 30});
    mAddButton->setText("+");

    setHeaderHidden(true);
    setModel(mModel);
    auto *delegate = new ItemDelegate(mModel);
    setItemDelegate(delegate);

    connect(this, &QTreeView::doubleClicked, this, &TreeView::onDoubleClick);
    connect(mAddButton, &QToolButton::clicked, this, &TreeView::onAddClicked);
    connect(mModel, &TreeViewModel::modelAboutToBeReset, this, &TreeView::onBeforeModelUpdate);
    connect(mModel, &TreeViewModel::modelReset, this, &TreeView::onModelUpdated);
    connect(delegate, &ItemDelegate::buttonClicked, this, &TreeView::onButtonClicked);
    connect(delegate,
            &ItemDelegate::needUpdate,
            this,
            qOverload<const QModelIndex &>(&QTreeView::update));

    setMouseTracking(true);
}

void TreeView::resizeEvent(QResizeEvent *event)
{
    QTreeView::resizeEvent(event);

    const QRect &geo = viewport()->geometry();
    QPoint point = geo.bottomRight();
    point.rx() -= mAddButton->width() + addButtonShift;
    point.ry() -= mAddButton->height() + addButtonShift;

    mAddButton->move(point);
}

void TreeView::onDoubleClick(const QModelIndex &index)
{
    if (!mModel->isChild(index)) {
        return;
    }

    auto operatorOpt = mModel->getOperator(index);
    if(!operatorOpt) {
        return;
    }

    EditDialog d(*operatorOpt, mDatabase, this);
    d.exec();
}

void TreeView::onButtonClicked(const QModelIndex &index)
{
    if (auto op = mModel->getOperator(index)) {
        onOperatorClicked(op->mcc, op->mnc);
    }
}

void TreeView::onAddClicked()
{
    AddDialog d(mDatabase, this);
    d.exec();
}

void TreeView::onBeforeModelUpdate()
{
    mExpandedItems.clear();

    for (const QModelIndex &index : mModel->getTopItems()) {
        if (isExpanded(index)) {
            mExpandedItems.push_back(mModel->getCountryName(index));
        }
    }
}

void TreeView::onModelUpdated()
{
    for (const QString &countryName : mExpandedItems) {
        QModelIndex index = mModel->getTopLevelIndex(countryName);
        if (index.isValid()) {
            expand(index);
        }
    }

    mExpandedItems.clear();
}

void TreeView::onOperatorClicked(int mcc, int mnc)
{
    qDebug() << "mcc" << mcc << "mnc" << mnc;
}
