#ifndef BOTROOM_H
#define BOTROOM_H

#include "botobject.h"
class QJsonObject;

class BotRoom : public BotObject
{
public:
    typedef std::shared_ptr<BotRoom> PTR;
    static PTR New(QJsonObject * jsonObject);

    //init by json
    BotRoom(QObject *parent = nullptr);
    BotRoom(QJsonObject * jsonObject);

    void InitByJson(QJsonObject * jsonObject);

    virtual QString Describe();

//    //title
//    void setTitle(const QString & Title){ title = Title; }
//    const QString & getTitle(){ return title; }
//    //type
//    void setType(const QString & Type){ type = Type; }
//    const QString & getType(){ return type; }
//
//    //isLocked
//    void Locked(bool islock = true);

    QString title;
    QString type;
    bool isLocked;
    QString teamId;
    QString lastActivity;
    QString creatorId;
    QString created;
    QString sipAddress;
};

#endif // BOTROOM_H
