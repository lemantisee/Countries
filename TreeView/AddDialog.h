#pragma once

#include <QDialog>

#include <QLineEdit>
#include <QLabel>

class Database;

class AddDialog : public QDialog
{
public:
    AddDialog(Database *database, QWidget *parent = nullptr);

private:
    void onAccept();
    void updateIcons(const QString &text);
    uint32_t getMcc() const;
    uint32_t getMnc() const;
    QWidget *createMccEdit();

    QLineEdit *mNameEdit = nullptr;
    QLineEdit *mMccEdit = nullptr;
    QLineEdit *mMncEdit = nullptr;
    QLabel *mIconLabel = nullptr;

    Database *mDatabase = nullptr;
};
