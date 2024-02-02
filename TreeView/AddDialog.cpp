#include "AddDialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QMessageBox>

#include "Database.h"

AddDialog::AddDialog(Database *database, QWidget *parent)
    :QDialog(parent), mDatabase(database)
{
    setWindowTitle(tr("Operator editor"));

    QFormLayout *form_l = new QFormLayout;

    mNameEdit = new QLineEdit;
    mMccEdit = new QLineEdit;
    mMncEdit = new QLineEdit;

    //TODO Add number validation
    form_l->addRow(tr("Name"), mNameEdit);
    form_l->addRow(tr("MCC"), mMccEdit);
    form_l->addRow(tr("MNC"), mMncEdit);

    QDialogButtonBox *btns = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

    QVBoxLayout *main_l = new QVBoxLayout(this);
    main_l->addLayout(form_l);
    main_l->addWidget(btns);

    connect(btns, &QDialogButtonBox::accepted, this, &AddDialog::onAccept);
    connect(btns, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void AddDialog::onAccept()
{

}
