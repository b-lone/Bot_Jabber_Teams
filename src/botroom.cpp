#include "botroom.h"

#include <QJsonObject>
#include <QDebug>

BotRoom::BotRoom(QJsonObject *jsonObject)
{
    if(jsonObject){
        this->id = jsonObject->value("id").toString();
        this->title = jsonObject->value("title").toString();
        this->type = jsonObject->value("type").toString();
        this->isLocked = jsonObject->value("isLocked").toBool();
        this->lastActivity = jsonObject->value("lastActivity").toString();
        this->creatorId = jsonObject->value("creatorId").toString();
        this->created = jsonObject->value("created").toString();
    }
}

QDebug operator <<(QDebug qd, BotRoom & room)
{
    return qd << "[BotRoom]\n{"
              << "\n    id:" << room.id
              << "\n    title:" << room.title
              << "\n    type:" << room.type
              << "\n    isLocked:" << room.isLocked
              << "\n    lastActivity:" << room.lastActivity
              << "\n    creatorId:" << room.creatorId
              << "\n    created:" << room.created
              << "\n}";
}
