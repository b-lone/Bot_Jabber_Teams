#include "botroom.h"

#include <QJsonObject>
#include <QDebug>

BotRoom::BotRoom(QObject *parent):BotObject (parent)
{
}

BotRoom::BotRoom(const QJsonObject jsonObjectPtr)
{
   InitFromJson(jsonObjectPtr);
}

void BotRoom::InitFromJson(const QJsonObject & jsonObjectPtr)
{
    this->id = jsonObjectPtr.value("id").toString();
    this->title = jsonObjectPtr.value("title").toString();
    this->type = jsonObjectPtr.value("type").toString();
    this->isLocked = jsonObjectPtr.value("isLocked").toBool();
    this->teamId = jsonObjectPtr.value("lastActivity").toString();
    this->lastActivity = jsonObjectPtr.value("lastActivity").toString();
    this->creatorId = jsonObjectPtr.value("creatorId").toString();
    this->created = jsonObjectPtr.value("created").toString();
    this->sipAddress = jsonObjectPtr.value("sipAddress").toString();

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
