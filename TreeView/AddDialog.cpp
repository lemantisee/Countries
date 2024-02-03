#include "AddDialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QMessageBox>
#include <QIntValidator>
#include <QLabel>

#include "Database.h"
#include "Icons.h"

AddDialog::AddDialog(Database *database, QWidget *parent)
    :QDialog(parent), mDatabase(database)
{
    setWindowTitle(tr("Operator editor"));
    setWindowIcon(Icons::getOperatorIcon(0, 0));

    mNameEdit = new QLineEdit;
    mMccEdit = new QLineEdit;
    mMccEdit->setValidator(new QIntValidator(0, 32765, this));

    mMncEdit = new QLineEdit;
    mMncEdit->setValidator(new QIntValidator(0, 32765, this));

    QFormLayout *form_l = new QFormLayout;

    form_l->addRow(tr("Name"), mNameEdit);
    form_l->addRow(tr("MCC"), createMccEdit());
    form_l->addRow(tr("MNC"), mMncEdit);

    QDialogButtonBox *btns = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

    QVBoxLayout *main_l = new QVBoxLayout(this);
    main_l->addLayout(form_l);
    main_l->addWidget(btns);

    connect(btns, &QDialogButtonBox::accepted, this, &AddDialog::onAccept);
    connect(btns, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(mMccEdit, &QLineEdit::textChanged, this, &AddDialog::updateIcons);
    connect(mMncEdit, &QLineEdit::textChanged, this, &AddDialog::updateIcons);
}

void AddDialog::onAccept()
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

    if (!mDatabase->addOperator(mcc, mnc, mNameEdit->text())) {
        QMessageBox::critical(this, tr("Error"), tr("Unable to add new operator"));
        return;
    }

    accept();
}

void AddDialog::updateIcons(const QString &)
{
    uint32_t mcc = getMcc();
    uint32_t mnc = getMnc();

    setWindowIcon(Icons::getOperatorIcon(mcc, mnc));

    const QString code = mDatabase->getCode(mcc);
    mIconLabel->setPixmap(Icons::getCountryIcon(code).pixmap({16, 16}));

}

uint32_t AddDialog::getMcc() const
{
    bool ok = false;
    uint32_t mcc = mMccEdit->text().toUInt(&ok);
    return ok ? mcc : 0;
}

uint32_t AddDialog::getMnc() const
{
    bool ok = false;
    uint32_t mnc = mMncEdit->text().toUInt(&ok);
    return ok ? mnc : 0;
}

QWidget *AddDialog::createMccEdit()
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
