#include "botwebhook.h"

#include <QJsonObject>
#include <QDebug>

BotWebhook::PTR BotWebhook::New(QJsonObject *jsonObject)
{
    PTR ptr(new BotWebhook);
    ptr->InitByJson(jsonObject);
    return ptr;
}

BotWebhook::BotWebhook(QObject *parent):BotObject (parent)
{

}

void BotWebhook::InitByJson(QJsonObject *jsonObject)
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
    //jsonObjectPtr->insert("status", this->status);
    //jsonObjectPtr->insert("filter", this->filter);
    //jsonObjectPtr->insert("secret", this->secret);
    return jsonObjectPtr;
}

QString BotWebhook::Describe()
{
    QString str;
    QTextStream ts(&str, QIODevice::WriteOnly);
    ts << "\n[BotWebhook]\n{"
       << "\n    id:" << id
       << "\n    name:" << name
       << "\n    targetUrl:" << targetUrl
       << "\n    resource:" << resource
       << "\n    event:" << event
       << "\n    filter:" << filter
       << "\n    secret:" << secret
       << "\n    status:" << status
       << "\n    created:" << created
       << "\n}";
    return str;
}
