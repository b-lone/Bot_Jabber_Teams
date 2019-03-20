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

   friend QDebug operator <<(QDebug os, BotRoom & br);

    enum Type{
        group,
        direct
    };

    QString id;
    QString title;
    Type type;
    bool isLocked;
    QString lastActivity;
    QString creatorId;
    QString created;
};

QDebug operator <<(QDebug qd, BotRoom & br);

#endif // BOTROOM_H
