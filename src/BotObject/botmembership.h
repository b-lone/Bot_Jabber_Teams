#ifndef BOTMEMBERSHIP_H
#define BOTMEMBERSHIP_H

#include "botobject.h"

class QJsonObject;


class BotMembership : public BotObject
{
public:
    //init by json
    BotMembership(QJsonObject *jsonObject);

    virtual QString Describe();

    QString roomID;
    QString personId;
    QString personEmail;
    QString personDisplayName;
    QString personOrgId;
    bool isModerator;
    bool isMonitor;
    QString created;
};

#endif // BOTMEMBERSHIP_H
