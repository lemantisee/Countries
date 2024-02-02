#pragma once

#include "CountryRecord.h"
#include <map>

class OperatorsSqlSelect
{
public:
    OperatorsSqlSelect();

    static std::string sql();

    bool processRow(int argc, char **argv, char **colName);
    std::vector<CountryRecord> getList() const;

private:
    struct RowResult {
        uint32_t mcc = 0;
        QString code;
        QString countryName;
        uint32_t mnc = 0;
        QString operatorName;
    };

    void appendRow(const RowResult &row);

    std::map<QString, CountryRecord> mRecords;
};
