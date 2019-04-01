#ifndef BOTWEBHOOK_H
#define BOTWEBHOOK_H

#include <memory>
#include <QString>
class QJsonObject;
//class QDebug;

class BotWebhook
{
public:
    BotWebhook() = default;
    BotWebhook(QJsonObject * jsonObject);

    std::shared_ptr<QJsonObject> Wrap();

    friend QDebug operator <<(QDebug qd, const BotWebhook & webhook);

    QString id;
    QString name;
    QString targetUrl;
    QString resource;
    QString event;
    QString filter;
    QString secret;
    QString status;
    QString created;
};

QDebug operator <<(QDebug qd, const BotWebhook & webhook);

#endif // BOTWEBHOOK_H
