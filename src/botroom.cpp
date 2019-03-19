#include "botroom.h"
#include <QJsonObject>

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
