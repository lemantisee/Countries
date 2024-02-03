#pragma once

#include <map>

#include <QIcon>

class Icons
{
public:
    static QIcon getOperatorIcon(uint32_t mcc, uint32_t mnc);
    static QIcon getCountryIcon(const QString &code);

private:
    Icons() = default;
    static Icons &getInstance();

    QIcon loadIcon(const QString &filepath);

    std::map<QString, QIcon> mIcons;
};
