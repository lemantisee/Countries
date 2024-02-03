#include "OperatorsSqlSelect.h"

#include <QDebug>

OperatorsSqlSelect::OperatorsSqlSelect() {}

std::string OperatorsSqlSelect::sql()
{
    return "SELECT countries.mcc, countries.code, countries.name, operators.mnc, "
           "operators.name as oname from countries JOIN operators ON countries.mcc=operators.mcc";
}

bool OperatorsSqlSelect::processRow(int argc, char **argv, char **colName)
{
    RowResult row;

    for (int i = 0; i < argc; i++) {
        const char *value = argv[i];
        if (!value) {
            qCritical() << "Invalid value";
            return false;
        }

        QString columnName = colName[i];

        if (columnName == "mcc") {
            row = {};
            QString valueStr = value;
            bool ok = false;
            int mccValue = valueStr.toInt(&ok);
            if (!ok) {
                qCritical() << "Invalid int value";
                return false;
            }

            row.mcc = mccValue;
        }

        if (columnName == "code") {
            row.code = value;
        }

        if (columnName == "name") {
            row.countryName = value;
        }

        if (columnName == "mnc") {
            QString valueStr = value;
            bool ok = false;
            int mncValue = valueStr.toInt(&ok);
            if (!ok) {
                qCritical() << "Invalid int value";
                return false;
            }

            row.mnc = mncValue;
        }

        if (columnName == "oname") {
            row.operatorName = value;
        }
    }

    appendRow(row);

    return true;
}

std::vector<CountryRecord> OperatorsSqlSelect::getList() const
{
    std::vector<CountryRecord> list;
    std::transform(mRecords.begin(),
                   mRecords.end(),
                   std::back_inserter(list),
                   [](const auto &cpair) { return cpair.second; });

    return list;
}

void OperatorsSqlSelect::appendRow(const RowResult &row)
{
    CountryRecord &country = mRecords[row.code];
    country.code = row.code;
    country.name = row.countryName;
    country.operators.emplace_back(row.mcc, row.mnc, row.operatorName);
}
