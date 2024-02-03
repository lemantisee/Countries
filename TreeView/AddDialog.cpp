#include "AddDialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QMessageBox>

#include "Database.h"

AddDialog::AddDialog(Database *database, QWidget *parent)
    :QDialog(parent), mDatabase(database)
{
    setWindowTitle(tr("Operator editor"));

    mNameEdit = new QLineEdit;
    mMccEdit = new QLineEdit;
    mMncEdit = new QLineEdit;

    QFormLayout *form_l = new QFormLayout;

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
    bool ok = false;
    uint32_t mcc = mMccEdit->text().toUInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, tr("Error"), tr("Invalid MCC value"));
        return;
    }

    ok = false;
    uint32_t mnc = mMncEdit->text().toUInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, tr("Error"), tr("Invalid MNC value"));
        return;
    }

    if (!mDatabase->addOperator(mcc, mnc, mNameEdit->text())) {
        QMessageBox::critical(this, tr("Error"), tr("Unable to add new operator"));
        return;
    }

    accept();
}
