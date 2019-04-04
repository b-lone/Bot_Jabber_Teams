#include "botroom.h"

#include <QTextStream>
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
