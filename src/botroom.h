#ifndef BOTROOM_H
#define BOTROOM_H

#include <QString>
class QJsonObject;

class BotRoom
{
public:
    //init by json
    BotRoom(QJsonObject *jsonObject);

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

#endif // BOTROOM_H
