#include "Database.h"

#include <QDebug>

#include "sqlite3.h"

namespace {
int callback(void *data, int argc, char **argv, char **azColName){

    for(int i = 0; i<argc; i++){
        qDebug() << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL");
    }

    return 0;
}
}

Database::Database(std::string_view filepath, QObject *parent)
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
    if (int rc = sqlite3_open(mFilepath.c_str(), &mDB); rc != SQLITE_OK) {
        qCritical() << "Unable to open database file " << mFilepath.c_str()
                    << ". Error:" << sqlite3_errmsg(mDB);
        sqlite3_close(mDB);
        return false;
    }

    char *zErrMsg = nullptr;
    if (int rc = sqlite3_exec(mDB, "SELECT * from countries", callback, nullptr, &zErrMsg);
        rc != SQLITE_OK) {
        qCritical() << "Unable to get countries. Error:" << zErrMsg;
        sqlite3_free(zErrMsg);
    }

    return true;
}
