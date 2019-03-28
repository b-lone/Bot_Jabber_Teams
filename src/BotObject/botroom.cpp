#include "botroom.h"

#include <QJsonObject>
#include <QDebug>

BotRoom::BotRoom(QObject *parent):BotObject (parent)
{
}

BotRoom::BotRoom(QJsonObject *jsonObject)
{
   InitFromJson(jsonObject);
}

void BotRoom::InitFromJson(QJsonObject *jsonObject)
{
    this->id = jsonObject->value("id").toString();
    this->title = jsonObject->value("title").toString();
    this->type = jsonObject->value("type").toString();
    this->isLocked = jsonObject->value("isLocked").toBool();
    this->teamId = jsonObject->value("lastActivity").toString();
    this->lastActivity = jsonObject->value("lastActivity").toString();
    this->creatorId = jsonObject->value("creatorId").toString();
    this->created = jsonObject->value("created").toString();
    this->sipAddress = jsonObject->value("sipAddress").toString();

}

QDebug operator <<(QDebug qd, BotRoom & room)
{
    return qd << "\n[BotRoom]\n{"
              << "\n    id:" << room.id
              << "\n    title:" << room.title
              << "\n    type:" << room.type
              << "\n    isLocked:" << room.isLocked
              << "\n    teamId:" << room.teamId
              << "\n    lastActivity:" << room.lastActivity
              << "\n    creatorId:" << room.creatorId
              << "\n    created:" << room.created
              << "\n    sipAddress:" << room.sipAddress
              << "\n}";
}
