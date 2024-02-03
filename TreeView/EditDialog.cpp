#include "EditDialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QMessageBox>

#include "Database.h"

EditDialog::EditDialog(Operator op, Database *database, QWidget *parent)
    : QDialog(parent), mOperator(std::move(op)), mDatabase(database)
{
    setWindowTitle(tr("Operator editor"));

    mNameEdit = new QLineEdit;
    mNameEdit->setText(mOperator.name);

    mMccEdit = new QLineEdit;
    mMccEdit->setReadOnly(true);
    mMccEdit->setText(QString::number(mOperator.mcc));

    mMncEdit = new QLineEdit;
    mMncEdit->setReadOnly(true);
    mMncEdit->setText(QString::number(mOperator.mnc));

    QFormLayout *form_l = new QFormLayout;

    form_l->addRow(tr("Name"), mNameEdit);
    form_l->addRow(tr("MCC"), mMccEdit);
    form_l->addRow(tr("MNC"), mMncEdit);

    QDialogButtonBox *btns = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

    QVBoxLayout *main_l = new QVBoxLayout(this);
    main_l->addLayout(form_l);
    main_l->addWidget(btns);

    connect(btns, &QDialogButtonBox::accepted, this, &EditDialog::onAccept);
    connect(btns, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void EditDialog::onAccept()
{
    if (mOperator.name == mNameEdit->text()) {
        reject();
        return;
    }

    if (mNameEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Empty name not allowed"));
        return;
    }

    if (!mDatabase->renameOperator(mOperator.mcc, mOperator.mnc, mNameEdit->text())) {
        QMessageBox::critical(this, tr("Error"), tr("Unable to change operator's name"));
        return;
    }

    accept();
}
