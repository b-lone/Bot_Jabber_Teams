#include "botwebhook.h"

#include <QJsonObject>
#include <QDebug>

BotWebhook::BotWebhook(QJsonObject *jsonObject)
{
    if(jsonObject){
        this->id = jsonObject->value("id").toString();
        this->name = jsonObject->value("name").toString();
        this->targetUrl = jsonObject->value("targetUrl").toString();
        this->resource = jsonObject->value("resource").toString();
        this->event = jsonObject->value("event").toString();
        this->filter = jsonObject->value("filter").toString();
        this->secret = jsonObject->value("secret").toString();
        this->status = jsonObject->value("status").toString();
        this->created = jsonObject->value("created").toString();
    }
}

std::shared_ptr<QJsonObject> BotWebhook::Wrap()
{
    std::shared_ptr<QJsonObject> jsonObjectPtr(new QJsonObject);
    jsonObjectPtr->insert("name", this->name);
    jsonObjectPtr->insert("targetUrl", this->targetUrl);
    jsonObjectPtr->insert("resource", this->resource);
    jsonObjectPtr->insert("event", this->event);
//    jsonObjectPtr->insert("filter", this->filter);
//    jsonObjectPtr->insert("secret", this->secret);
    return jsonObjectPtr;
}

QDebug operator <<(QDebug qd, const BotWebhook &webhook)
{
    return qd << "\n[BotWebhook]\n{"
              << "\n    id:" << webhook.id
              << "\n    name:" << webhook.name
              << "\n    targetUrl:" << webhook.targetUrl
              << "\n    resource:" << webhook.resource
              << "\n    event:" << webhook.event
              << "\n    filter:" << webhook.filter
              << "\n    secret:" << webhook.secret
              << "\n    status:" << webhook.status
              << "\n    created:" << webhook.created
              << "\n}";
}
