#ifndef BOTROOM_H
#define BOTROOM_H

#include "botobject.h"
class QJsonObject;

class BotRoom : public BotObject
{
public:
    //init by json
    BotRoom(QObject *parent = nullptr);
    BotRoom(QJsonObject  * jsonObject);

    void InitFromJson(QJsonObject * jsonObject);

    //title
    void setTitle(const QString & Title){ title = Title; }
    const QString & getTitle(){ return title; }
    //type
    void setType(const QString & Type){ type = Type; }
    const QString & getType(){ return type; }

    //isLocked
    void Locked(bool islock = true);

    friend QDebug operator <<(QDebug os, BotRoom & rooom);

    QString title;
    QString type;
    bool isLocked;
    QString teamId;
    QString lastActivity;
    QString creatorId;
    QString created;
    QString sipAddress;
};

QDebug operator <<(QDebug qd, BotRoom & rooom);

class BotRoomStore
{
public:
};

#endif // BOTROOM_H
