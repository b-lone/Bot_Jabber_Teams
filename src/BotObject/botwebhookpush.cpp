#include "botwebhookpush.h"

#include <QJsonObject>
#include <QDebug>

#include "botcommon.h"
#include "botmessage.h"
#include "botmembership.h"
#include "botroom.h"
#include "botconfig.h"

BotWebhookPush::PTR BotWebhookPush::New(QJsonObject *jsonObject)
{
    PTR ptr(new BotWebhookPush);
    ptr->InitByJson(jsonObject);
    return ptr;
}

BotWebhookPush::BotWebhookPush(QObject *parent):BotObject (parent)
{

}

void BotWebhookPush::InitByJson(QJsonObject *jsonObject)
{
    if(jsonObject){
        ParseJsonToString(id);
        ParseJsonToString(resource);
        ParseJsonToString(event);
        ParseJsonToString(filter);
        ParseJsonToString(orgId);
        ParseJsonToString(createdBy);
        ParseJsonToString(appId);
        ParseJsonToString(ownedBy);
        ParseJsonToString(status);
        ParseJsonToString(actorId);

        ParseJsonToObject(data);
        if(resource == "messages"){
            object = BotMessage::New(&dataObject);
        }else if(resource == "rooms"){
            object = BotRoom::New(&dataObject);
        }else if(resource == "memberships"){
            object = BotMembership::New(&dataObject);
        }
    }
}

bool BotWebhookPush::IsMessageEventFromBot()
{
    auto botId = S_CONFIG->Value(BotConfig::BotId).toString();
    auto obj = std::dynamic_pointer_cast<BotMessage>(object);
    if(obj){
        return obj->personId == botId;
    }
    return false;
}

QString BotWebhookPush::Describe()
{
//    QString str;
//    QTextStream ts(&str, QIODevice::WriteOnly);
//    ts << "\n[BotWebhook]\n{"
//       << "\n    id:" << id
//       << "\n    name:" << name
//       << "\n    targetUrl:" << targetUrl
//       << "\n    resource:" << resource
//       << "\n    event:" << event
//       << "\n    filter:" << filter
//       << "\n    secret:" << secret
//       << "\n    status:" << status
//       << "\n    created:" << created
//       << "\n}";
//    return str;
}
