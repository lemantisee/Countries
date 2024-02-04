#include "EditDialog.h"

#include <QMessageBox>

#include "Database.h"

EditDialog::EditDialog(Operator op, Database *database, QWidget *parent)
    : OperatorDialog(database, parent)
    , mOperator(std::move(op))
    , mDatabase(database)
{
    setName(mOperator.name);
    setMcc(mOperator.mcc);
    setMnc(mOperator.mnc);
}

void EditDialog::accepting()
{
    if (mOperator.name == getName()) {
        reject();
        return;
    }

    if (!mDatabase->renameOperator(mOperator.mcc, mOperator.mnc, getName())) {
        QMessageBox::critical(this, tr("Error"), tr("Unable to change operator's name"));
        return;
    }

    accept();
}
