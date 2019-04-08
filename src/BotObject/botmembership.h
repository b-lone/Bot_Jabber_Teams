#ifndef BOTMEMBERSHIP_H
#define BOTMEMBERSHIP_H

#include "botobject.h"

class QJsonObject;


class BotMembership : public BotObject
{
public:
    typedef std::shared_ptr<BotMembership> PTR;
    static PTR New(QJsonObject * jsonObject);

    //init by json
    BotMembership(QObject *parent = nullptr);
    BotMembership(QJsonObject *jsonObject);

    void InitByJson(QJsonObject * jsonObject);

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
