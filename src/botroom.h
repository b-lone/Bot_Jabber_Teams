#ifndef BOTROOM_H
#define BOTROOM_H

#include <QString>
class QJsonObject;
class QDebug;

class BotRoom
{
public:
    //init by json
    BotRoom(QJsonObject *jsonObject);

   friend QDebug operator <<(QDebug os, BotRoom & rooom);

    QString id;
    QString title;
    QString type;
    bool isLocked;
    QString lastActivity;
    QString creatorId;
    QString created;
};

QDebug operator <<(QDebug qd, BotRoom & rooom);

#endif // BOTROOM_H
