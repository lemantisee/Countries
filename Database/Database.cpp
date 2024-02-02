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

    // for(int i = 0; i<argc; i++){



    //     const char *value = argv[i];
    //     if (!value) {
    //         return 1;
    //     }



    //     QString columnName = colName[i];
    //     if (columnName == "mcc") {

    //         QString valueStr = value;
    //         bool ok = false;
    //         int mcc = valueStr.toInt(&ok);
    //         if (!ok) {
    //             return 1;
    //         }

    //         *countryRow = resultMap[mcc];
    //     }

    //     if (!countryRow->addField(columnName, argv[i])) {
    //         break;
    //     }

    //     if (columnName == "mnc_length") {
    //         table->push_back(countryRow);
    //     }
    // }

    // return 0;
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
    // std::vector<CountryRecord> table;
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

// bool Country::addField(const QString &columnName, char *value)
// {
//     if (!value) {
//         return false;
//     }

//     if (columnName == "mcc") {
//         QString valueStr = value;
//         bool ok = false;
//         int mccValue = valueStr.toInt(&ok);
//         if (!ok) {
//             return false;
//         }

//         mcc = mccValue;
//         return true;
//     }

//     if (columnName == "name") {
//         name = value;
//         return true;
//     }

//     if (columnName == "code") {
//         code = value;
//         return true;
//     }

//     return true;
// }
