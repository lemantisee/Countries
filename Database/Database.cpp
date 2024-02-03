#include "Database.h"

#include <QDebug>

#include "sqlite3.h"
#include "OperatorsSqlSelect.h"

namespace {
int countriesTableCallback(void *data, int argc, char **argv, char **colName){

    OperatorsSqlSelect *selectProcessor = reinterpret_cast<OperatorsSqlSelect *>(data);

    if (!selectProcessor->processRow(argc, argv, colName)) {
        qCritical() << "Unable to process sql result";
        return 1;
    }

    return 0;
}
}

Database::Database(const QString &filepath, QObject *parent)
    : QObject{parent}
    , mFilepath(filepath)
{}

Database::~Database()
{
    if (mDB) {
        sqlite3_close(mDB);
    }

    mDB = nullptr;
}

bool Database::open()
{
    if (int rc = sqlite3_open(mFilepath.toStdString().c_str(), &mDB); rc != SQLITE_OK) {
        qCritical() << "Unable to open database file " << mFilepath.toStdString().c_str()
                    << ". Error:" << sqlite3_errmsg(mDB);
        sqlite3_close(mDB);
        return false;
    }

    return true;
}

bool Database::renameOperator(uint32_t mmc, uint32_t mnc, const QString &newName)
{
    QString sql = QString("UPDATE operators SET name = '%1' WHERE mcc = %2 AND mnc = %3")
                      .arg(newName)
                      .arg(mmc)
                      .arg(mnc);

    char *zErrMsg = nullptr;
    int rc = sqlite3_exec(mDB, sql.toStdString().c_str(), nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        qCritical() << "Unable to update operator name. Error:" << zErrMsg;
        sqlite3_free(zErrMsg);
        return false;
    }

    emit updated();

    return true;
}

bool Database::addOperator(uint32_t mmc, uint32_t mnc, const QString &name)
{
    QString sql = QString("INSERT INTO operators (mcc, mnc, name) VALUES (%1, %2, '%3')")
                      .arg(mmc)
                      .arg(mnc)
                      .arg(name);

    char *zErrMsg = nullptr;
    int rc = sqlite3_exec(mDB, sql.toStdString().c_str(), nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        qCritical() << "Unable to add new operator. Error:" << zErrMsg;
        sqlite3_free(zErrMsg);
        return false;
    }

    emit updated();

    return true;
}

std::vector<CountryRecord> Database::getCountries()
{
    OperatorsSqlSelect selectProcessor;
    char *zErrMsg = nullptr;

    int rc = sqlite3_exec(mDB,
                          OperatorsSqlSelect::sql().c_str(),
                          countriesTableCallback,
                          &selectProcessor,
                          &zErrMsg);
    if (rc != SQLITE_OK) {
        qCritical() << "Unable to get countries. Error:" << zErrMsg;
        sqlite3_free(zErrMsg);
        return {};
    }

    std::vector<CountryRecord> countries = selectProcessor.getList();

    updateMccCodeMap(countries);

    return countries;
}

QString Database::getCode(uint32_t mmc) const
{
    auto it = mMccToCodeMap.find(mmc);
    return it == mMccToCodeMap.end() ? QString() : it->second;
}

void Database::updateMccCodeMap(const std::vector<CountryRecord> &records)
{
    mMccToCodeMap.clear();

    for (const CountryRecord &country : records) {
        for (const Operator &op : country.operators) {
            mMccToCodeMap[op.mcc] = country.code;
        }
    }
}
