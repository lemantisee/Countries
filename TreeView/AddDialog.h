#pragma once

#include <QDialog>

#include <QLineEdit>

class Database;

class AddDialog : public QDialog
{
public:
    AddDialog(Database *database, QWidget *parent = nullptr);

private:
    void onAccept();

    QLineEdit *mNameEdit = nullptr;
    QLineEdit *mMccEdit = nullptr;
    QLineEdit *mMncEdit = nullptr;

    Database *mDatabase = nullptr;
};
