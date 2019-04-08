#include "botnetworkcontroller.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "botcommon.h"
#include "botroom.h"
#include "botmembership.h"
#include "botmessage.h"
#include "botwebhook.h"
#include "botpeople.h"

BotNetworkController::BotNetworkController(QObject *parent) : QObject(parent)
{

}

void BotNetworkController::on_dataReady(std::shared_ptr<QByteArray> data, RequestType requestType)
{
    BOTLOG("Recive http data.");

    QJsonObject * jsonObject = new QJsonObject;
    if(ParseBytesToJson(data, jsonObject)){
        if(jsonObject->contains("items")){
            BOTLOG("Multi objects.");
            QJsonArray subArray = jsonObject->value("items").toArray();
            for (int i = 0; i< subArray.size(); i++) {
                QJsonObject subObject = subArray[i].toObject();
                Distribute(&subObject, requestType);
            }
        }else {
            BOTLOG("Single object.");
            Distribute(jsonObject, requestType);
        }
    }
    delete  jsonObject;
}

void BotNetworkController::Distribute(QJsonObject *jsonObject, RequestType requestType)
{
    switch (requestType) {
    case RequestType::rooms:
        Rooms(jsonObject);
        break;
    case RequestType::people:
        People(jsonObject);
        break;
    case RequestType::messages:
        Messages(jsonObject);
        break;
    case RequestType::webhooks:
        Webhooks(jsonObject);
        break;
    case RequestType::memberships:
        Memberships(jsonObject);
        break;
    }
}

void BotNetworkController::Rooms(QJsonObject *jsonObject)
{
    BOTLOG("New room object.");
    if(jsonObject){
        BotRoom::PTR object = BotRoom::New(jsonObject);
        emit roomReady(object);
    }
}

void BotNetworkController::People(QJsonObject *jsonObject)
{
    BOTLOG("New people object.");
    if(jsonObject){
        BotPeople::PTR object = BotPeople::New(jsonObject);
        emit peopleReady(object);
    }
}

void BotNetworkController::Messages(QJsonObject *jsonObject)
{
    BOTLOG("New message object.");
    if(jsonObject){
        BotMessage::PTR object = BotMessage::New(jsonObject);
        emit messageReady(object);
    }
}

void BotNetworkController::Webhooks(QJsonObject *jsonObject)
{
    BOTLOG("New webhook object.");
    if(jsonObject){
        BotWebhook::PTR object = BotWebhook::New(jsonObject);
        emit webhookReady(object);
    }
}

void BotNetworkController::Memberships(QJsonObject *jsonObject)
{
    BOTLOG("New membership object.");
    if(jsonObject){
        BotMembership::PTR object = BotMembership::New(jsonObject);
        emit membershipReady(object);
    }
}

//void BotNetworkController::on_getObject(std::shared_ptr<BotObject> object)
//{
//    BotRoom::PTR cptr = std::dynamic_pointer_cast<BotRoom>(object);
//    if(cptr)
//}
