#pragma once

#include "OperatorDialog.h"

#include <QLineEdit>
#include <QLabel>

class Database;

class AddDialog : public OperatorDialog
{
public:
    AddDialog(Database *database, QWidget *parent = nullptr);

protected:
    void accepting() override;

private:
    Database *mDatabase = nullptr;
};
