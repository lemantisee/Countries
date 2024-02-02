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

std::vector<CountryRecord> Database::getCountries() const
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

    return selectProcessor.getList();
}
