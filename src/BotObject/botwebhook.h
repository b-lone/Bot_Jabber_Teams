#ifndef BOTWEBHOOK_H
#define BOTWEBHOOK_H
#include "botobject.h"

#include <memory>
#include <QString>
class QJsonObject;

class BotWebhook : public BotObject
{
public:
    BotWebhook() = default;
    BotWebhook(QJsonObject * jsonObject);

    std::shared_ptr<QJsonObject> Wrap();

    virtual QString Describe();

    QString name;
    QString targetUrl;
    QString resource;
    QString event;
    QString filter;
    QString secret;
    QString status;
    QString created;
};

#endif // BOTWEBHOOK_H
