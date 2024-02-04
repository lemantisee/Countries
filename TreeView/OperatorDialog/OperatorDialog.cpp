#include "OperatorDialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QMessageBox>
#include <QLabel>
#include <QPixmap>
#include <QValidator>
#include <QPushButton>

#include "Database.h"
#include "Icons.h"

OperatorDialog::OperatorDialog(Database *database, QWidget *parent)
    : QDialog(parent)
    , mDatabase(database)
{
    setWindowTitle(tr("Operator editor"));

    mNameEdit = new QLineEdit;
    mMccEdit = new QLineEdit;
    mMccEdit->setValidator(new QIntValidator(0, 32765, this));
    mMccEdit->setText("0");
    mMccEdit->setReadOnly(true);

    mMncEdit = new QLineEdit;
    mMncEdit->setValidator(new QIntValidator(0, 32765, this));
    mMncEdit->setText("0");
    mMncEdit->setReadOnly(true);

    QFormLayout *form_l = new QFormLayout;

    form_l->addRow(tr("Name"), mNameEdit);
    form_l->addRow(tr("MCC"), createMccEdit());
    form_l->addRow(tr("MNC"), mMncEdit);

    QDialogButtonBox *btns = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    mSaveButton = btns->button(QDialogButtonBox::Save);
    mSaveButton->setEnabled(false);

    QVBoxLayout *main_l = new QVBoxLayout(this);
    main_l->addLayout(form_l);
    main_l->addWidget(btns);

    connect(btns, &QDialogButtonBox::accepted, this, &OperatorDialog::accepting);
    connect(btns, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(mNameEdit, &QLineEdit::textChanged, this, [this](const QString &){
        validate();
    });
    connect(mMccEdit, &QLineEdit::textChanged, this, [this](const QString &){
        validate();
        updateIcons({});
    });
    connect(mMncEdit, &QLineEdit::textChanged, this, &OperatorDialog::updateIcons);

    validate();
    updateIcons({});
}

void OperatorDialog::setMcc(uint32_t mcc)
{
    mMccEdit->setText(QString::number(mcc));
}

void OperatorDialog::setMnc(uint32_t mnc)
{
    mMncEdit->setText(QString::number(mnc));
}

void OperatorDialog::setName(const QString &name)
{
    mNameEdit->setText(name);
}

void OperatorDialog::enableEditing(bool state)
{
    mMccEdit->setReadOnly(!state);
    mMncEdit->setReadOnly(!state);
}

QWidget *OperatorDialog::createMccEdit()
{
    mIconLabel = new QLabel;
    mIconLabel->setPixmap(Icons::getCountryIcon("").pixmap({16, 16}));

    QWidget *wrapper = new QWidget;
    QHBoxLayout *wrapper_l = new QHBoxLayout(wrapper);
    wrapper_l->setContentsMargins({});
    wrapper_l->addWidget(mMccEdit);
    wrapper_l->addWidget(mIconLabel);

    return wrapper;
}

void OperatorDialog::updateIcons(const QString &)
{
    uint32_t mcc = getMcc();
    uint32_t mnc = getMnc();

    setWindowIcon(Icons::getOperatorIcon(mcc, mnc));

    const QString code = mDatabase->getCode(mcc);
    mIconLabel->setPixmap(Icons::getCountryIcon(code).pixmap({16, 16}));
}

uint32_t OperatorDialog::getMcc() const
{
    bool ok = false;
    uint32_t mcc = mMccEdit->text().toUInt(&ok);
    return ok ? mcc : 0;
}

uint32_t OperatorDialog::getMnc() const
{
    bool ok = false;
    uint32_t mnc = mMncEdit->text().toUInt(&ok);
    return ok ? mnc : 0;
}

QString OperatorDialog::getName() const
{
    return mNameEdit->text();
}

void OperatorDialog::validate()
{
    mSaveButton->setEnabled(true);
    mNameEdit->setStyleSheet({});
    mMccEdit->setStyleSheet({});

    if (mNameEdit->text().isEmpty()) {
        mNameEdit->setStyleSheet("border: 1px solid red");
        mSaveButton->setEnabled(false);
    }

    if (mDatabase->getCode(getMcc()).isEmpty()) {
        mMccEdit->setStyleSheet("border: 1px solid red");
        mSaveButton->setEnabled(false);
    }
}
