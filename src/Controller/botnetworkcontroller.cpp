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
        switch (requestType) {
        case RequestType::rooms:
            RoomRoot(jsonObject);
            break;
        case RequestType::people:
            PeopleRoot(jsonObject);
            break;
        case RequestType::messages:
            MessageRoot(jsonObject);
            break;
        case RequestType::webhooks:
            WebhookRoot(jsonObject);
            break;
        case RequestType::memberships:
            MembershipRoot(jsonObject);
            break;
        }
    }
    delete  jsonObject;
}

void BotNetworkController::on_ngrokReady(std::shared_ptr<QByteArray> data)
{
    BOTLOG("Recive ngrok data.");

    QJsonObject * jsonObject = new QJsonObject;
    if(ParseBytesToJson(data, jsonObject)){
        if(jsonObject->contains("tunnels")){
            QJsonArray subArray = jsonObject->value("tunnels").toArray();
            for (int i = 0; i< subArray.size(); i++) {
                QJsonObject subObject = subArray[i].toObject();
                if(subObject.value("proto").toString() == "https"){
                    auto public_url = subObject.value("public_url").toString();
                    BOTLOG(public_url);
                    emit ngrokUrlReady(public_url);
                    break;
                }
            }
        }
    }
}

void BotNetworkController::RoomRoot(QJsonObject *jsonObject)
{
    if(jsonObject->contains("items")){
        Rooms(jsonObject);
    }else {
        Room(jsonObject);
    }
}

void BotNetworkController::PeopleRoot(QJsonObject *jsonObject)
{
    if(jsonObject->contains("items")){
        People(jsonObject);
    }else {
        Person(jsonObject);
    }
}

void BotNetworkController::MessageRoot(QJsonObject *jsonObject)
{
    if(jsonObject->contains("items")){
        Messages(jsonObject);
    }else {
        Message(jsonObject);
    }
}

void BotNetworkController::WebhookRoot(QJsonObject *jsonObject)
{
    if(jsonObject->contains("items")){
        Webhooks(jsonObject);
    }else {
        Webhook(jsonObject);
    }
}

void BotNetworkController::MembershipRoot(QJsonObject *jsonObject)
{
    if(jsonObject->contains("items")){
        Memberships(jsonObject);
    }else {
        Membership(jsonObject);
    }
}

template<class T>
std::shared_ptr<T> BotNetworkController::GenerateObject(QJsonObject *jsonObject)
{
    if(jsonObject){
        std::shared_ptr<T> object = T::New(jsonObject);
        return object;
    }
    return nullptr;
}

template<class T>
std::vector<std::shared_ptr<T>> BotNetworkController::GenerateObjects(QJsonObject *jsonObject)
{
    std::vector<std::shared_ptr<T>> result;
    if(jsonObject){
        QJsonArray subArray = jsonObject->value("items").toArray();
        for (int i = 0; i< subArray.size(); i++) {
            QJsonObject subObject = subArray[i].toObject();
            auto object = GenerateObject<T>(&subObject);
            result.push_back(object);
        }
        return result;
    }
    return result;
}

void BotNetworkController::Rooms(QJsonObject *jsonObject)
{
    BOTLOG("Rooms");
    if(jsonObject){
        auto objects = GenerateObjects<BotRoom>(jsonObject);
        emit roomListReady(objects);
    }
}

void BotNetworkController::People(QJsonObject *jsonObject)
{
    BOTLOG("People");
    if(jsonObject){
        auto objects = GenerateObjects<BotPeople>(jsonObject);
        emit peopleListReady(objects);
    }
}

void BotNetworkController::Messages(QJsonObject *jsonObject)
{
    BOTLOG("Messages");
    if(jsonObject){
        auto objects = GenerateObjects<BotMessage>(jsonObject);
        emit messageListReady(objects);
    }
}

void BotNetworkController::Webhooks(QJsonObject *jsonObject)
{
    BOTLOG("Webhooks");
    if(jsonObject){
        auto objects = GenerateObjects<BotWebhook>(jsonObject);
        emit webhookListReady(objects);
    }
}

void BotNetworkController::Memberships(QJsonObject *jsonObject)
{
    BOTLOG("Memberships");
    if(jsonObject){
        auto objects = GenerateObjects<BotMembership>(jsonObject);
        emit membershipListReady(objects);
    }
}

void BotNetworkController::Room(QJsonObject *jsonObject)
{
    BOTLOG("Room");
    if(jsonObject){
        auto object = GenerateObject<BotRoom>(jsonObject);
        emit roomReady(object);
    }
}

void BotNetworkController::Person(QJsonObject *jsonObject)
{
    BOTLOG("Person");
    if(jsonObject){
        auto object = GenerateObject<BotPeople>(jsonObject);
        emit peopleReady(object);
    }
}

void BotNetworkController::Message(QJsonObject *jsonObject)
{
    BOTLOG("Message");
    if(jsonObject){
        auto object = GenerateObject<BotMessage>(jsonObject);
        emit messageReady(object);
    }
}

void BotNetworkController::Webhook(QJsonObject *jsonObject)
{
    BOTLOG("Webhook");
    if(jsonObject){
        auto object = GenerateObject<BotWebhook>(jsonObject);
        emit webhookReady(object);
    }
}

void BotNetworkController::Membership(QJsonObject *jsonObject)
{
    BOTLOG("Membership");
    if(jsonObject){
        auto object = GenerateObject<BotMembership>(jsonObject);
        emit membershipReady(object);
    }
}
