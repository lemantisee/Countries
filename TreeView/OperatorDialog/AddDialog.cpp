#include "AddDialog.h"

#include <QMessageBox>

#include "Database.h"

AddDialog::AddDialog(Database *database, QWidget *parent)
    : OperatorDialog(database, parent)
    , mDatabase(database)
{
    enableEditing(true);
}

void AddDialog::accepting()
{
    uint32_t mcc = getMcc();
    if (mcc == 0) {
        QMessageBox::warning(this, tr("Error"), tr("Invalid MCC value"));
        return;
    }

    uint32_t mnc = getMnc();
    if (mnc == 0) {
        QMessageBox::warning(this, tr("Error"), tr("Invalid MNC value"));
        return;
    }

    if (!mDatabase->addOperator(mcc, mnc, getName())) {
        QMessageBox::critical(this, tr("Error"), tr("Unable to add new operator"));
        return;
    }

    accept();
}
