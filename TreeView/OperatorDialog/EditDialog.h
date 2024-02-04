#pragma once

#include "OperatorDialog.h"

#include <QLineEdit>

#include "CountryRecord.h"

class Database;

class EditDialog : public OperatorDialog
{
public:
    EditDialog(Operator op, Database *database, QWidget *parent = nullptr);

protected:
    void accepting() override;

private:
    Operator mOperator;
    Database *mDatabase = nullptr;
};
