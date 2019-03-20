#ifndef BOTMEMBERSHIP_H
#define BOTMEMBERSHIP_H

#include <QString>
class QJsonObject;
class QDebug;


class BotMembership
{
public:
    //init by json
    BotMembership(QJsonObject *jsonObject);

   friend QDebug operator <<(QDebug os, BotMembership & membership);

    QString id;
    QString roomID;
    QString personId;
    QString personEmail;
    QString personDisplayName;
    QString personOrgId;
    bool isModerator;
    bool isMonitor;
    QString created;
};

QDebug operator <<(QDebug qd, BotMembership & membership);

#endif // BOTMEMBERSHIP_H
