#include "botpingcontroller.h"

#include <QTimer>
#include "botcommon.h"
#include "bothttpclient.h"
#include "botmessage.h"

BotPingController::BotPingController(QObject *parent) : QObject(parent)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &BotPingController::onTimer);

    message = new BotMessage(this);
}

BotPingController::~BotPingController()
{
    if(timer){
        if(timer->isActive()){
            BOTLOG("stop timer.");
            timer->stop();
        }
    }
}

void BotPingController::Init(std::shared_ptr<BotMessage> msg)
{
    message->roomId = msg->roomId;
}

QString BotPingController::getRoomId()
{
    return message->roomId;
}

void BotPingController::StartPing(int timerInterval)
{
    timer->start(timerInterval);
}

void BotPingController::onTimer()
{
    message->text = "Ping " + QString::number(count++);
    BOTLOG(message->text);
    S_HTTPCLIENT->sendCreateMessage(*message);
}
