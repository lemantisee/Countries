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
    bool renameOperator(uint32_t mmc, uint32_t mnc, const QString &newName);
    bool addOperator(uint32_t mmc, uint32_t mnc, const QString &name);

    std::vector<CountryRecord> getCountries();
    QString getCode(uint32_t mmc) const;

signals:
    void updated();

private:
    void updateMccCodeMap(const std::vector<CountryRecord> &records);

    sqlite3 *mDB = nullptr;
    QString mFilepath;
    std::map<uint32_t, QString> mMccToCodeMap;
};
