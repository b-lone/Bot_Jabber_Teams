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

QDebug operator <<(QDebug qd, BotMembership & membership)
{
    return qd << "\n[BotMembership]\n{"
              << "\n    id:" << membership.id
              << "\n    roomID:" << membership.roomID
              << "\n    personId:" << membership.personId
              << "\n    personEmail:" << membership.personEmail
              << "\n    personDisplayName:" << membership.personDisplayName
              << "\n    personOrgId:" << membership.personOrgId
              << "\n    isModerator:" << membership.isModerator
              << "\n    isMonitor:" << membership.isMonitor
              << "\n    created:" << membership.created
              << "\n}";
}
