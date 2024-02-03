#include "EditDialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QMessageBox>
#include <QLabel>
#include <QPixmap>

#include "Database.h"
#include "Icons.h"

EditDialog::EditDialog(Operator op, Database *database, QWidget *parent)
    : QDialog(parent), mOperator(std::move(op)), mDatabase(database)
{
    setWindowTitle(tr("Operator editor"));
    setWindowIcon(Icons::getOperatorIcon(mOperator.mcc, mOperator.mnc));

    mNameEdit = new QLineEdit;
    mNameEdit->setText(mOperator.name);

    mMccEdit = new QLineEdit;
    mMccEdit->setReadOnly(true);
    mMccEdit->setText(QString::number(mOperator.mcc));

    mMncEdit = new QLineEdit;
    mMncEdit->setReadOnly(true);
    mMncEdit->setText(QString::number(mOperator.mnc));

    QFormLayout *form_l = new QFormLayout;

    form_l->addRow(tr("Name"), mNameEdit);
    form_l->addRow(tr("MCC"), createMccEdit());
    form_l->addRow(tr("MNC"), mMncEdit);

    QDialogButtonBox *btns = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

    QVBoxLayout *main_l = new QVBoxLayout(this);
    main_l->addLayout(form_l);
    main_l->addWidget(btns);

    connect(btns, &QDialogButtonBox::accepted, this, &EditDialog::onAccept);
    connect(btns, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void EditDialog::onAccept()
{
    if (mOperator.name == mNameEdit->text()) {
        reject();
        return;
    }

    if (!mDatabase->renameOperator(mOperator.mcc, mOperator.mnc, mNameEdit->text())) {
        QMessageBox::critical(this, tr("Error"), tr("Unable to change operator's name"));
        return;
    }

    accept();
}

QWidget *EditDialog::createMccEdit()
{
    const QString &code = mDatabase->getCode(mOperator.mcc);
    auto *iconLabel = new QLabel;
    iconLabel->setPixmap(Icons::getCountryIcon(code).pixmap({16, 16}));

    QWidget *wrapper = new QWidget;
    QHBoxLayout *wrapper_l = new QHBoxLayout(wrapper);
    wrapper_l->setContentsMargins({});
    wrapper_l->addWidget(mMccEdit);
    wrapper_l->addWidget(iconLabel);

    return wrapper;
}
