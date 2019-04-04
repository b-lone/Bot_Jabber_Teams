#include "botnetworkcontroller.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "botcommon.h"
#include "botroom.h"

BotNetworkController::BotNetworkController(QObject *parent) : QObject(parent)
{

}

void BotNetworkController::on_dataReady(const QByteArray & data, RequestType requestType)
{
    BOTLOG("Recive http data.");

    QJsonObject * jsonObject{};
    if(ParseBytesToJson(data, jsonObject)){
        if(jsonObject->contains("items")){
            QJsonArray subArray = jsonObject->value("items").toArray();
            for (int i = 0; i< subArray.size(); i++) {
                QJsonObject subObject = subArray[i].toObject();
                Distribute(&subObject, requestType);
            }
        }else {
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
    
}

void BotNetworkController::People(QJsonObject *jsonObject)
{
    
}

void BotNetworkController::Messages(QJsonObject *jsonObject)
{
    
}

void BotNetworkController::Webhooks(QJsonObject *jsonObject)
{
    
}

void BotNetworkController::Memberships(QJsonObject *jsonObject)
{
    
}

//void BotNetworkController::on_getObject(std::shared_ptr<BotObject> object)
//{
//    BotRoom::PTR cptr = std::dynamic_pointer_cast<BotRoom>(object);
//    if(cptr)
//}
