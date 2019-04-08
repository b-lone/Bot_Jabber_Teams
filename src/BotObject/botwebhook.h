#ifndef BOTWEBHOOK_H
#define BOTWEBHOOK_H
#include "botobject.h"

#include <memory>
#include <QString>
class QJsonObject;

class BotWebhook : public BotObject
{
public:
    typedef std::shared_ptr<BotWebhook> PTR;
    static PTR New(QJsonObject * jsonObject);

    BotWebhook(QObject *parent = nullptr);
    BotWebhook(QJsonObject * jsonObject);

    void InitByJson(QJsonObject * jsonObject);

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
