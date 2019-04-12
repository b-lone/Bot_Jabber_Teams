#ifndef BOTWEBHOOK_H
#define BOTWEBHOOK_H
#include "botobject.h"

#include <memory>

class QJsonObject;

class BotWebhookPush : public BotObject
{
public:
    typedef std::shared_ptr<BotWebhookPush> PTR;
    static PTR New(QJsonObject * jsonObject);

    BotWebhookPush(QObject *parent = nullptr);

    void InitByJson(QJsonObject * jsonObject);

    bool IsMessageEventFromBot();

    virtual QString Describe();

    QString name;
    QString resource;
    QString event;
    QString filter;
    QString orgId;
    QString createdBy;
    QString appId;
    QString ownedBy;
    QString status;
    QString actorId;
    std::shared_ptr<BotObject> object;
};

#endif // BOTWEBHOOK_H
