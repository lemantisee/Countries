#pragma once

#include <QDialog>

#include <QLineEdit>

#include "CountryRecord.h"

class Database;

class EditDialog : public QDialog
{
public:
    EditDialog(Operator op, Database *database, QWidget *parent = nullptr);

private:
    void onAccept();

    QLineEdit *mNameEdit = nullptr;
    QLineEdit *mMccEdit = nullptr;
    QLineEdit *mMncEdit = nullptr;

    Operator mOperator;
    Database *mDatabase = nullptr;
};
