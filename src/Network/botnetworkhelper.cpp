#include "botnetworkhelper.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include "botcommon.h"
#include "botconfig.h"

QString RequestTypeToString(RequestType type)
{
    static QMap<RequestType, QString> queryList = {
        {RequestType::memberships, "memberships"},
        {RequestType::messages, "messages"},
        {RequestType::people, "people"},
        {RequestType::rooms, "rooms"},
        {RequestType::webhooks, "webhooks"},
    };
    return queryList[type];
}

//BotNetworkReplyHelper
BotNetworkReplyHelper::BotNetworkReplyHelper(QNetworkReply *nr): QObject(nr), networkReply(nr)
{
    connect(nr, &QNetworkReply::finished, this, &BotNetworkReplyHelper::on_finished);
}

void BotNetworkReplyHelper::on_finished()
{
    emit finished(this);
}

//BotNetworkRepquestHelper
const QString BotNetworkRepquestHelper::mUrl = "https://api.ciscospark.com/v1/";
const QString BotNetworkRepquestHelper::contentType_Json = "application/json";

BotNetworkRepquestHelper::PTR BotNetworkRepquestHelper::New(RequestType rt, SendType st)
{
    return std::make_shared<BotNetworkRepquestHelper>(rt, st);
}

BotNetworkRepquestHelper::BotNetworkRepquestHelper(RequestType rt, SendType st): requestType(rt), sendType(st)
{
    this->request = std::make_shared<QNetworkRequest>();

    auto botAccessToken = BOTCONFIG->Value(BotConfig::BotAccessToken).toByteArray();
    request->setRawHeader("Authorization", botAccessToken);
}

std::shared_ptr<QNetworkRequest> BotNetworkRepquestHelper::GenerateRequest()
{
    QString sUrl = mUrl + RequestTypeToString(this->requestType);
    if(objectId != ""){
        sUrl += "/" + objectId;
    }
    if(params.count()){
        sUrl += "?";
        auto keys = params.keys();
        bool flag = false;
        for(auto key : keys){
            if(flag){
                sUrl += "&";
            }else{
                flag = true;
            }

            sUrl = sUrl + key + "=" + params[key];
        }
    }
    BOTLOG(sUrl);
    request->setUrl(sUrl);
    return request;
}

void BotNetworkRepquestHelper::setContentType(const QString &contentType)
{
    this->request->setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, contentType);
}

QString BotNetworkRepquestHelper::getContentType()
{
    return this->request->header(QNetworkRequest::KnownHeaders::ContentTypeHeader).toString();
}

void BotNetworkRepquestHelper::setBodyByte(const QJsonObject &jsonObject)
{
    bodyByte = QJsonDocument(jsonObject).toJson();
}
