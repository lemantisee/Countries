#pragma once

#include <QString>

struct Operator {
    uint32_t mcc = 0;
    uint32_t mnc = 0;
    QString name;
};

class CountryRecord
{
public:
    CountryRecord() = default;

    QString name;
    uint32_t mcc = 0;
    QString code;

    std::vector<Operator> operators;
};
