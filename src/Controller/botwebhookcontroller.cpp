#include "botwebhookcontroller.h"

#include <QJsonObject>
#include <QJsonArray>
#include "botnetworkmanager.h"
#include "botcommon.h"
#include "botwebhook.h"


//BOTLOG("Creat new webhook:" << sURl);
//BotWebhook webhook;
//webhook.name = "Kun You";
//webhook.targetUrl = sURl;
//webhook.resource = "messages";
//webhook.event = "created";
//this->sendCreateWebhook(webhook);

BotWebhookController::BotWebhookController(QObject *parent) : QObject(parent)
{
    connect(BOTNETWORKMANAGER, &BotNetworkManager::ngrokReady, this, &BotWebhookController::on_ngrokReady);
}

bool BotWebhookController::ParseData(std::shared_ptr<QByteArray> data, QString &URL)
{
    QJsonObject * jsonObject = new QJsonObject;
    if(ParseBytesToJson(data, jsonObject)){
        if(jsonObject->contains("tunnels")){
            auto tunnelsObj = jsonObject->value("tunnels").toArray();
            for (auto tunnel: tunnelsObj) {
                auto tunnelObj = tunnel.toObject();
                if(tunnelObj.value("proto").toString() == "https"){
                    URL = tunnelObj.value("public_url").toString();
                    BOTLOG("URL:" << URL);
                    return true;
                }
            }
        }
    }

    return false;
}

void BotWebhookController::on_ngrokReady(std::shared_ptr<QByteArray> data)
{
    BOTLOG("Recive http data.");

    QString URL;
    if(ParseData(data, URL)){
        ngrokUrl = URL;
        BOTNETWORKMANAGER->sendListWebhooks();
    }
}

void BotWebhookController::on_webhookReady(std::shared_ptr<BotWebhook> object)
{
    if(ngrokUrl == ""){
        BOTNETWORKMANAGER->sendDelete(RequestType::webhooks, object->id);
    }
    if(object->targetUrl != ngrokUrl){

    }
}

void BotWebhookController::on_emptyList(RequestType rt)
{

}
