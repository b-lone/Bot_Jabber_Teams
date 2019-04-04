#include "botmembership.h"

#include <QJsonObject>
#include <QDebug>

BotMembership::BotMembership(QJsonObject *jsonObject)
{
    if(jsonObject){
        this->id = jsonObject->value("id").toString();
        this->roomID = jsonObject->value("roomID").toString();
        this->personId = jsonObject->value("personId").toString();
        this->personEmail = jsonObject->value("personEmail").toString();
        this->personDisplayName = jsonObject->value("personDisplayName").toString();
        this->personOrgId = jsonObject->value("personOrgId").toString();
        this->isModerator = jsonObject->value("isModerator").toBool();
        this->isMonitor = jsonObject->value("isMonitor").toBool();
        this->created = jsonObject->value("created").toString();
    }
}

QString BotMembership::Describe()
{
    QString str;
    QTextStream ts(&str, QIODevice::WriteOnly);
    ts << "\n[BotMembership]\n{"
       << "\n    id:" << id
       << "\n    roomID:" << roomID
       << "\n    personId:" << personId
       << "\n    personEmail:" << personEmail
       << "\n    personDisplayName:" << personDisplayName
       << "\n    personOrgId:" << personOrgId
       << "\n    isModerator:" << isModerator
       << "\n    isMonitor:" << isMonitor
       << "\n    created:" << created
       << "\n}";
    return str;
}
