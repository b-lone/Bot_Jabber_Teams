#include "botwebhookcontroller.h"

#include "botnetworkmanager.h"
#include "botcommon.h"


//BOTLOG("Creat new webhook:" << sURl);
//BotWebhook webhook;
//webhook.name = "Kun You";
//webhook.targetUrl = sURl;
//webhook.resource = "messages";
//webhook.event = "created";
//this->sendCreateWebhook(webhook);

BotWebhookController *BotWebhookController::Instance()
{
    static BotWebhookController * botWC = new BotWebhookController();
    return botWC;
}

BotWebhookController::BotWebhookController(QObject *parent) : QObject(parent)
{
    connect(BOTNETWORKMANAGER, &BotNetworkManager::getNgrokURL, this, &BotWebhookController::on_getNgrokURL);
}

void BotWebhookController::on_getNgrokURL(QString url)
{
    BOTLOG("Deal with Ngrok URL");
    if(newNgrokUrl == url)
        return;


}
