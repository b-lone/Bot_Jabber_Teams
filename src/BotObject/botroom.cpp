#include "botroom.h"

#include <QTextStream>
#include <QJsonObject>
#include <QDebug>

BotRoom::PTR BotRoom::New(QJsonObject *jsonObject)
{
    PTR ptr(new BotRoom);
    ptr->InitByJson(jsonObject);
    return ptr;
}

BotRoom::BotRoom(QObject *parent):BotObject (parent)
{
}

void BotRoom::InitByJson(QJsonObject *jsonObject)
{
    if(jsonObject){
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

}

QString BotRoom::Describe()
{
    QString str;
    QTextStream ts(&str, QIODevice::WriteOnly);
    ts << "\n[BotRoom]\n{"
       << "\n    id:" << id
       << "\n    title:" << title
       << "\n    type:" << type
       << "\n    isLocked:" << isLocked
       << "\n    teamId:" << teamId
       << "\n    lastActivity:" << lastActivity
       << "\n    creatorId:" << creatorId
       << "\n    created:" << created
       << "\n    sipAddress:" << sipAddress
       << "\n}";
    return str;
}
