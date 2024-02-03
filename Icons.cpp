#include "Icons.h"

#include <QFileInfo>

namespace {
Icons *instance = nullptr;

const QString operatorFolder = "Icons/Operators";
const QString countryFolder = "Icons/Countries";
}

QIcon Icons::getOperatorIcon(uint32_t mcc, uint32_t mnc)
{
    QString filename = QString("%1_%2.png").arg(mcc).arg(mnc);
    return getInstance().loadIcon(operatorFolder + "/" + filename);
}

QIcon Icons::getCountryIcon(const QString &code)
{
    QString filename = code + ".png";
    return getInstance().loadIcon(countryFolder + "/" + filename);
}

Icons &Icons::getInstance()
{
    if (!instance) {
        instance = new Icons;
    }

    return *instance;
}

QIcon Icons::loadIcon(const QString &filepath)
{
    auto it = mIcons.find(filepath);
    if (it != mIcons.end()) {
        return it->second;
    }

    if (!QFileInfo::exists(filepath)) {
        return QIcon(":/img/question.png");
    }

    QIcon icon(filepath);
    mIcons[filepath] = icon;

    return icon;
}
