#include "botwebhookpushcontroller.h"

#include "bothttpclient.h"
#include "botcommon.h"
#include "bottcpserver.h"
#include "bottcpservercontroller.h"
#include "bothttpclientcontroller.h"
#include "botwebhookpush.h"
#include "botmessage.h"
#include "botconfig.h"

BotWebhookPushController::BotWebhookPushController(QObject *parent) : QObject(parent)
{
    auto tcpcontroller = S_TCPSERVER->getTcpServerController();
    connect(tcpcontroller, &BotTcpServerController::webhookPushReady, this, &BotWebhookPushController::on_webhookPushReady);

    auto httpcontroller = S_HTTPCLIENT->getNetworkController();
    connect(httpcontroller, &BotHttpClientController::messageReady, this, &BotWebhookPushController::on_messageReady);
}

void BotWebhookPushController::on_webhookPushReady(std::shared_ptr<BotWebhookPush> object)
{
    BOTLOG("on_webhookPushReady");
    if(CheckExist(object)){
        if(object->resource == "messages"){
            ProcessMessages(object);
        }else if (object->resource == "rooms") {

        }else if (object->resource == "memberships") {

        }
    }
}

void BotWebhookPushController::on_messageReady(std::shared_ptr<BotMessage> object)
{
    BOTLOG("on_messageReady");
    if(CheckExistAndRemove(object->id, "created")){
        S_HTTPCLIENT->sendCreateMessage(*object);
    }
}

bool BotWebhookPushController::CheckExist(std::shared_ptr<BotWebhookPush> object)
{
    BOTLOG("CheckExist");

    auto itr = std::find_if(webhookPushStore.begin(),
                            webhookPushStore.end(),
                            [&](std::shared_ptr<BotWebhookPush> obj){ return obj->id == object->id;});
    return itr == webhookPushStore.end();
}

void BotWebhookPushController::ProcessMessages(std::shared_ptr<BotWebhookPush> object)
{
    BOTLOG("ProcessMessages");
    if(object->event == "created"){
        ProcessMessagesCreated(object);
    }else if(object->event == "updated"){

    }else if(object->event == "deleted"){

    }
}

void BotWebhookPushController::ProcessMessagesCreated(std::shared_ptr<BotWebhookPush> object)
{
    BOTLOG("ProcessMessagesCreated");
    if(!object->IsMessageEventFromBot()){
        webhookPushStore.push_back(object);
        S_HTTPCLIENT->sendGetDetails(RequestType::messages, object->object->id);
    }
}

bool BotWebhookPushController::CheckExistAndRemove(QString id, QString event)
{
    auto itr = std::find_if(webhookPushStore.begin(),
                            webhookPushStore.end(),
                            [&](std::shared_ptr<BotWebhookPush> obj){ return (obj->object->id == id)&&(obj->event == event);});
    if(itr == webhookPushStore.end()){
        return false;
    }else {
        webhookPushStore.erase(itr);
        return true;
    }
}

//void BotTcpServer::OnNewMessage(std::shared_ptr<BotMessage> message)
//{
//    BOTLOG("New Message");
//    for (int i = 0; i< messageIds.length(); ++i) {
//        if(messageIds[i] == message->id){
//            BotMessage msgForSend;
//            msgForSend.roomId = message->roomId;

//            auto text = message->text;
//            if(message->roomType == "group"){
//                auto displayName = BOTCONFIG->Value(BotConfig::BotDisplayName).toString();
//                text = message->text.mid(displayName.count());
//            }
//            if(text.contains("picture")){// remove(' ') == "sendmepicture"){
//                msgForSend.files.push_back("picture/hello.png");
//            }else {
//                msgForSend.text = text;
//            }


//            S_HTTPCLIENT->sendCreateMessage(msgForSend);
//            messageIds.removeAll(message->id);
//        }
//    }
//}
