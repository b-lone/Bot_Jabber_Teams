#include "botcommand.h"

#include <QJsonObject>
#include "botcommon.h"
#include "botmessage.h"
#include "botmembership.h"
#include "botroom.h"

BotCommand::PTR BotCommand::New(QJsonObject *jsonObject)
{
    PTR ptr(new BotCommand);
    ptr->InitByJson(jsonObject);
    return ptr;
}

BotCommand::BotCommand(QObject *parent) : QObject(parent)
{

}

void BotCommand::InitByJson(QJsonObject *jsonObject)
{
    if(jsonObject){
        ParseJsonToString(resource);
        ParseJsonToString(event);
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
/*
{
"resource":"messages",
"event":"creat",
"data":{
"toPersonEmail":"kuyou@cisco.com",
"text":"hello"
}
}
*/
