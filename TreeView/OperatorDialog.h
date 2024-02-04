#pragma once

#include <QDialog>

#include <QLineEdit>
#include <QLabel>

class Database;

class OperatorDialog : public QDialog
{
public:
    OperatorDialog(Database *database, QWidget *parent = nullptr);

protected:
    virtual void accepting() = 0;

    void setMcc(uint32_t mcc);
    void setMnc(uint32_t mnc);
    void setName(const QString &name);

    uint32_t getMcc() const;
    uint32_t getMnc() const;
    QString getName() const;

private:
    void validate();
    QWidget *createMccEdit();
    void updateIcons(const QString &);

    QLineEdit *mNameEdit = nullptr;
    QLineEdit *mMccEdit = nullptr;
    QLineEdit *mMncEdit = nullptr;
    QLabel *mIconLabel = nullptr;
    QPushButton *mSaveButton = nullptr;

    Database *mDatabase = nullptr;
};
