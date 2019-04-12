#include "bottcpservercontroller.h"
#include <QByteArray>
#include <QJsonObject>

#include "botconfig.h"
#include "botcommon.h"
#include "botwebhookpush.h"

BotTcpServerController::BotTcpServerController(QObject *parent) : QObject(parent)
{

}

void BotTcpServerController::ProcessingNgrokData(std::shared_ptr<QByteArray> data)
{
    BOTLOG("ProcessingNgrokData");
    if(data->isEmpty())
        return;

    auto arrayList = data->split('\n');
    auto lastObjectOfArray = arrayList.end() - 1;

    QJsonObject * jsonObject = new QJsonObject;
    if(ByteArrayToJson(*lastObjectOfArray, jsonObject)){
        auto object = BotWebhookPush::New(jsonObject);
        emit webhookPushReady(object);
    }
    delete  jsonObject;
}
