#include "botroom.h"

#include <QJsonObject>
#include <QDebug>

BotRoom::BotRoom(QJsonObject *jsonObject)
{
    if(jsonObject){
        this->id = jsonObject->value("id").toString();
        this->title = jsonObject->value("title").toString();
        {
            auto sType = jsonObject->value("type").toString();
            if(sType == "group"){
                this->type = Type::group;
            }
            else if (sType == "direct") {
                this->type = Type::direct;
            }
        }
        this->isLocked = jsonObject->value("isLocked").toBool();
        this->lastActivity = jsonObject->value("lastActivity").toString();
        this->creatorId = jsonObject->value("created").toString();
        this->created = jsonObject->value("creatorId").toString();
    }
}

QDebug operator <<(QDebug qd, BotRoom & br)
{
    return qd << "[BotRoom]\n{"
              << "\n    id:" << br.id
              << "\n    title:" << br.title
              << "\n    type:" << br.type
              << "\n    isLocked:" << br.isLocked
              << "\n    lastActivity:" << br.lastActivity
              << "\n    creatorId:" << br.creatorId
              << "\n    created:" << br.created
              << "\n}";
}
