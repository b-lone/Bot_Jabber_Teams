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

    void InitByJson(QJsonObject * jsonObject);

    std::shared_ptr<QJsonObject> Wrap();

    bool operator ==(const BotWebhook & other);
    bool operator !=(const BotWebhook & other);
    virtual QString Describe();

    QString name;
    QString targetUrl;
    QString resource;
    QString event;
    QString filter;
    QString secret;
    QString status;// active inactive
    QString created;
};

#endif // BOTWEBHOOK_H
