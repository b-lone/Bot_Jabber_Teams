#include "botcommandcontroller.h"

#include "bottcpserver.h"
#include "bottcpservercontroller.h"
#include "botcommand.h"
#include "botcommon.h"
#include "bothttpclient.h"
#include "botmessage.h"

BotCommandController::BotCommandController(QObject *parent) : QObject(parent)
{
    auto tcpcontroller = S_TCPSERVER->getTcpServerController();
    connect(tcpcontroller, &BotTcpServerController::commandReady, this, &BotCommandController::on_commandReady);
}

void BotCommandController::ProcessMessages(std::shared_ptr<BotCommand> object)
{
    BOTLOG("ProcessMessages");
    if(object->event == "creat"){
        ProcessMessagesCreated(object);
    }else if(object->event == "update"){

    }else if(object->event == "delete"){

    }
}

void BotCommandController::ProcessMessagesCreated(std::shared_ptr<BotCommand> object)
{
    auto message = std::dynamic_pointer_cast<BotMessage>(object->object);
    if(message){
        S_HTTPCLIENT->sendCreateMessage(*message);
    }
}

void BotCommandController::on_commandReady(std::shared_ptr<BotCommand> object)
{
    BOTLOG("on_webhookPushReady");
    if(object){
        if(object->resource == "messages"){
            ProcessMessages(object);
        }else if (object->resource == "rooms") {

        }else if (object->resource == "memberships") {

        }
    }
}
