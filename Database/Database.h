#pragma once

#include <QObject>

#include "CountryRecord.h"

struct sqlite3;

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(const QString &filepath, QObject *parent = nullptr);
    ~Database();

    bool open();

    std::vector<CountryRecord> getCountries() const;

private:
    sqlite3 *mDB = nullptr;
    QString mFilepath;
};
