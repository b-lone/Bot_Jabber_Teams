#include "botnetworkmanager.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QHttpMultiPart>
#include <QFile>
#include <QFileInfo>
#include "botconfig.h"
#include "botroom.h"
#include "botmembership.h"
#include "botmessage.h"
#include "botpeople.h"
#include "botwebhook.h"
#include "botcommon.h"

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

BotNetworkRepquestHelper::BotNetworkRepquestHelper(BotNetworkRepquestHelper::RequestType type): requestType(type)
{
    this->request = std::make_shared<QNetworkRequest>();

    auto botAccessToken = CONFIG->Value(BotConfig::AccessToken).toByteArray();
    request->setRawHeader("Authorization", botAccessToken);
}

std::shared_ptr<QNetworkRequest> BotNetworkRepquestHelper::GennerateRequest()
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
    BOTOUT(sUrl);
    request->setUrl(sUrl);
    return request;
}

void BotNetworkRepquestHelper::setContentType(const QString &contentType)
{
    this->request->setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, contentType);
}

QString BotNetworkRepquestHelper::RequestTypeToString(BotNetworkRepquestHelper::RequestType type)
{
    static QMap<RequestType, QString> queryList = {
        {RequestType::Memberships, "memberships"},
        {RequestType::Messages, "messages"}
    };
    return queryList[type];
}

//BotNetworkManager
BotNetworkManager::BotNetworkManager()
{
    networkAccessManager = std::make_shared<QNetworkAccessManager>(this);
}

BotNetworkManager *BotNetworkManager::Instance()
{
    static BotNetworkManager * botNetworkManager = new BotNetworkManager();
    return botNetworkManager;
}

void BotNetworkManager::sendGetMemberships(QString roomId, QString filterString, bool isFilterByPersonId)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::Memberships);
    if(roomId != ""){
        networkRequestHelper.setParams("roomId",roomId);
        if(filterString != ""){
            if(isFilterByPersonId){
                networkRequestHelper.setParams("personId", filterString);
            }else {
                networkRequestHelper.setParams("personEmail", filterString);
            }
        }
    }

    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Get, request, &BotNetworkManager::on_GetMemberships);
}

void BotNetworkManager::sendCreateAMembership(QString roomId, bool isModerator, QString byString, bool isByEmail)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::Memberships);
    networkRequestHelper.setContentType(BotNetworkRepquestHelper::contentType_Json);
    auto request = networkRequestHelper.GennerateRequest();

    QJsonObject jsonObject;
    jsonObject.insert("roomId", roomId);
    jsonObject.insert("isModerator", isModerator);
    if(isByEmail){
        jsonObject.insert("personEmail", byString);
    }else {
        jsonObject.insert("personId", byString);
    }

    SendAndConnect(SendType::Post, request, &BotNetworkManager::on_CreateAMembership, jsonObject);
}

void BotNetworkManager::sendGetMembership(QString membershipId)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::Memberships);
    networkRequestHelper.setObjectId(membershipId);
    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Get, request, &BotNetworkManager::on_GetMembership);
}

void BotNetworkManager::sendUpdateMembership(QString membershipId, bool isModerator)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::Memberships);
    networkRequestHelper.setObjectId(membershipId);
    networkRequestHelper.setContentType(BotNetworkRepquestHelper::contentType_Json);
    auto request = networkRequestHelper.GennerateRequest();

    QJsonObject jsonObject;
    jsonObject.insert("isModerator", isModerator);

    SendAndConnect(SendType::Put, request, &BotNetworkManager::on_UpdateMembership, jsonObject);
}

void BotNetworkManager::sendDeleteMembership(QString membershipId)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::Memberships);
    networkRequestHelper.setObjectId(membershipId);
    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Delete, request, &BotNetworkManager::on_DeleteMembership);
}

void BotNetworkManager::sendListMessages(QString roomId, QString mentionedPeople, QString before, QString beforeMessage, int max)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::Messages);
    networkRequestHelper.setParams("roomId", roomId);
    if(mentionedPeople != ""){
        networkRequestHelper.setParams("mentionedPeople", mentionedPeople);
    }
    if(before != ""){
        networkRequestHelper.setParams("before", before);
    }
    if(beforeMessage != ""){
        networkRequestHelper.setParams("beforeMessage", beforeMessage);
    }
    networkRequestHelper.setParams("max", QString::number(max));
    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Get, request, &BotNetworkManager::on_ListMessages);
}

void BotNetworkManager::sendListDirectMessages(QString byString, bool isByEmail)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::Messages);
    networkRequestHelper.setObjectId("direct");
    if(isByEmail){
        networkRequestHelper.setParams("personEmail", byString);
    }else {
        networkRequestHelper.setParams("personId", byString);
    }
    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Get, request, &BotNetworkManager::on_ListDirectMessages);
}

void BotNetworkManager::sendCreateMessage(QString roomId, QString text, QString files)
{
    auto request = this->NewRequest(QUrl("https://api.ciscospark.com/v1/messages"));


    QNetworkReply * reply;

    if(files == "")
    {
        request->setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

        QJsonObject jsonObject;
        jsonObject.insert("roomId", roomId);
        jsonObject.insert("text", text);
//        jsonObject.insert("markdown", "**PROJECT UPDATE** A new project plan has been published [on Box](http://box.com/s/lf5vj)."
//        "The PM for this project is <@personEmail:mike@example.com> and the Engineering Manager is <@personEmail:jane@example.com>.");

        reply = networkAccessManager->post(*(request.get()), QJsonDocument(jsonObject).toJson(QJsonDocument::JsonFormat::Compact));
    }else {
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

        {
            QHttpPart textPart;
            textPart.setHeader(QNetworkRequest::KnownHeaders::ContentDispositionHeader, "form-data; name=\"roomId\"");
            textPart.setBody(roomId.toUtf8());

            multiPart->append(textPart);
        }
        {
            QHttpPart imagePart;
            imagePart.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "image/png");
            imagePart.setHeader(QNetworkRequest::KnownHeaders::ContentDispositionHeader, "form-data; name=\"files\"; filename =\"image.png\"");

            QFile * file = new QFile(files);
            file->open(QIODevice::ReadOnly);
            imagePart.setBodyDevice(file);
            file->setParent(multiPart);

            multiPart->append(imagePart);
        }
        reply = networkAccessManager->post(*(request.get()), multiPart);
        multiPart->setParent(reply);
    }

    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_CreateMessage);
}

void BotNetworkManager::sendGetMessageDetails(QString messageId)
{
    QString surl = QString("https://api.ciscospark.com/v1/messages/") + messageId;
    auto request = this->NewRequest(QUrl(surl));

    QNetworkReply * reply = networkAccessManager->get(*(request.get()));
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_GetMessageDetails);
}

void BotNetworkManager::sendDeleteMessage(QString messageId)
{
    QString surl = QString("https://api.ciscospark.com/v1/messages/") + messageId;
    auto request = this->NewRequest(QUrl(surl));

    QNetworkReply * reply = networkAccessManager->deleteResource(*(request.get()));
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_DeleteMessage);
}

void BotNetworkManager::sendListPeople(QString email)
{
    QString surl = QString("https://api.ciscospark.com/v1/people?email=") + email;
    auto request = this->NewRequest(QUrl(surl));

    QNetworkReply * reply = networkAccessManager->get(*(request.get()));
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_ListPeople);
}

void BotNetworkManager::sendGetPersonDetails(QString personId)
{
    QString surl = QString("https://api.ciscospark.com/v1/people/") + personId;
    auto request = this->NewRequest(QUrl(surl));

    QNetworkReply * reply = networkAccessManager->get(*(request.get()));
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_GetPersonDetails);
}

void BotNetworkManager::sendListRooms()
{
    QString surl = QString("https://api.ciscospark.com/v1/rooms");
    auto request = this->NewRequest(QUrl(surl));

    QNetworkReply * reply = networkAccessManager->get(*(request.get()));
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_ListRooms);
}

void BotNetworkManager::sendCreateRoom(QString title)
{
    QString surl = QString("https://api.ciscospark.com/v1/rooms/");
    auto request = this->NewRequest(QUrl(surl));
    request->setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

    QJsonObject jsonObject;
    jsonObject.insert("title", title);

    QNetworkReply * reply = networkAccessManager->post(*(request.get()), QJsonDocument(jsonObject).toJson());
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_CreateRoom);
}

void BotNetworkManager::sendGetRoomDetails(QString roomId)
{
    QString surl = QString("https://api.ciscospark.com/v1/rooms/") + roomId;
    auto request = this->NewRequest(QUrl(surl));

    QNetworkReply * reply = networkAccessManager->get(*(request.get()));
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_GetRoomDetails);
}

void BotNetworkManager::sendUpdateRoom(QString roomId, QString title)
{
    QString surl = QString("https://api.ciscospark.com/v1/rooms/") + roomId;
    auto request = this->NewRequest(QUrl(surl));
    request->setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

    QJsonObject jsonObject;
    jsonObject.insert("title", title);

    QNetworkReply * reply = networkAccessManager->put(*(request.get()), QJsonDocument(jsonObject).toJson());
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_UpdateRoom);
}

void BotNetworkManager::sendDeleteRoom(QString roomId)
{
    QString surl = QString("https://api.ciscospark.com/v1/rooms/") + roomId;
    auto request = this->NewRequest(QUrl(surl));

    QNetworkReply * reply = networkAccessManager->deleteResource(*(request.get()));
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_DeleteRoom);
}

void BotNetworkManager::sendListWebhooks()
{
    auto request = this->NewRequest(QUrl("https://api.ciscospark.com/v1/webhooks"));

    QNetworkReply * reply = networkAccessManager->get(*(request.get()));
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_ListWebhooks);
}

void BotNetworkManager::sendCreateWebhook(BotWebhook & webhook)
{
    auto request = this->NewRequest(QUrl("https://api.ciscospark.com/v1/webhooks"));
    request->setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

    auto jsonObjectPtr = webhook.Wrap();

    QNetworkReply * reply = networkAccessManager->post(*request, QJsonDocument(*jsonObjectPtr).toJson());
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_CreateWebHook);
}

void BotNetworkManager::sendGetWebhookDetails(QString webhookId)
{
    QString surl = QString("https://api.ciscospark.com/v1/webhooks/") + webhookId;
    auto request = this->NewRequest(QUrl(surl));

    QNetworkReply * reply = networkAccessManager->get(*(request.get()));
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_GetWebhookDetails);
}

void BotNetworkManager::sendUpdateWebhook(QString webhookId, QString name)
{
    QString surl = QString("https://api.ciscospark.com/v1/webhooks/") + webhookId;
    auto request = this->NewRequest(QUrl(surl));
    request->setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

    QJsonObject jsonObject;
    jsonObject.insert("name", name);
    jsonObject.insert("targetUrl", "https://6eca506c.ngrok.io");

    QNetworkReply * reply = networkAccessManager->put(*(request.get()), QJsonDocument(jsonObject).toJson());
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_UpdateWebhook);
}

void BotNetworkManager::sendDeleteWebhook(QString webhookId)
{
    QString surl = QString("https://api.ciscospark.com/v1/webhooks/") + webhookId;
    auto request = this->NewRequest(QUrl(surl));

    QNetworkReply * reply = networkAccessManager->deleteResource(*(request.get()));
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_DeleteWebhook);
}

void BotNetworkManager::on_GetMemberships(BotNetworkReplyHelper *nrh)
{
    QJsonObject rootObj = this->ExtractContect(nrh);

    if(rootObj.contains("items"))
    {
        QJsonArray subArray = rootObj.value("items").toArray();
        for(int i = 0; i< subArray.size(); i++)
        {
            QJsonObject subObject = subArray.at(i).toObject();
            auto botmembership = new BotMembership(&subObject);
            qDebug() << *botmembership;
        }
    }
}

void BotNetworkManager::on_CreateAMembership(BotNetworkReplyHelper *nrh)
{
    QJsonObject rootObj = this->ExtractContect(nrh);

    auto botmembership = new BotMembership(&rootObj);
    qDebug() << *botmembership;
}

void BotNetworkManager::on_GetMembership(BotNetworkReplyHelper *nrh)
{
    on_CreateAMembership(nrh);
}

void BotNetworkManager::on_UpdateMembership(BotNetworkReplyHelper *nrh)
{
    on_CreateAMembership(nrh);
}

void BotNetworkManager::on_DeleteMembership(BotNetworkReplyHelper *nrh)
{
    this->ExtractContect(nrh);
}

void BotNetworkManager::on_ListMessages(BotNetworkReplyHelper *nrh)
{
    QJsonObject rootObj = this->ExtractContect(nrh);

    if(rootObj.contains("items"))
    {
        QJsonArray subArray = rootObj.value("items").toArray();
        for(int i = 0; i< subArray.size(); i++)
        {
            QJsonObject subObject = subArray.at(i).toObject();
            auto botmessage = new BotMessage(&subObject);
            qDebug() << *botmessage;
        }
    }
}

void BotNetworkManager::on_ListDirectMessages(BotNetworkReplyHelper *nrh)
{
    on_ListMessages(nrh);
}

void BotNetworkManager::on_CreateMessage(BotNetworkReplyHelper *nrh)
{
    QJsonObject rootObj = this->ExtractContect(nrh);

    auto botmessage= new BotMessage(&rootObj);
    qDebug() << *botmessage;
}

void BotNetworkManager::on_GetMessageDetails(BotNetworkReplyHelper *nrh)
{
    on_CreateMessage(nrh);
}

void BotNetworkManager::on_DeleteMessage(BotNetworkReplyHelper *nrh)
{
    this->ExtractContect(nrh);
}

void BotNetworkManager::on_ListPeople(BotNetworkReplyHelper *nrh)
{
    QJsonObject rootObj = this->ExtractContect(nrh);

    if(rootObj.contains("items"))
    {
        QJsonArray subArray = rootObj.value("items").toArray();
        for(int i = 0; i< subArray.size(); i++)
        {
            QJsonObject subObject = subArray.at(i).toObject();
            auto botpeople = new BotPeople(&subObject);
            qDebug() << *botpeople;
        }
    }
}

void BotNetworkManager::on_GetPersonDetails(BotNetworkReplyHelper *nrh)
{
    QJsonObject rootObj = this->ExtractContect(nrh);

    auto botpeople = new BotPeople(&rootObj);
    qDebug() << *botpeople;
}

void BotNetworkManager::on_ListRooms(BotNetworkReplyHelper *nrh)
{
    QJsonObject rootObj = this->ExtractContect(nrh);

    if(rootObj.contains("items"))
    {
        QJsonArray subArray = rootObj.value("items").toArray();
        for(int i = 0; i< subArray.size(); i++)
        {
            QJsonObject subObject = subArray.at(i).toObject();
//            auto botroom = new BotRoom(subObject);
//            qDebug() << *botroom;
        }
    }
}

void BotNetworkManager::on_CreateRoom(BotNetworkReplyHelper *nrh)
{
    QJsonObject rootObj = this->ExtractContect(nrh);

//    auto botroom = new BotRoom(rootObj);
//    qDebug() << *botroom;
}

void BotNetworkManager::on_GetRoomDetails(BotNetworkReplyHelper *nrh)
{
    QJsonObject rootObj = this->ExtractContect(nrh);

//    auto botroom = new BotRoom(rootObj);
//    qDebug() << *botroom;
}

void BotNetworkManager::on_UpdateRoom(BotNetworkReplyHelper *nrh)
{
    QJsonObject rootObj = this->ExtractContect(nrh);

//    auto botroom = new BotRoom(rootObj);
//    qDebug() << *botroom;
}

void BotNetworkManager::on_DeleteRoom(BotNetworkReplyHelper *nrh)
{
   this->ExtractContect(nrh);
}

void BotNetworkManager::on_ListWebhooks(BotNetworkReplyHelper *nrh)
{
    QJsonObject rootObj = this->ExtractContect(nrh);

    if(rootObj.contains("items"))
    {
        QJsonArray subArray = rootObj.value("items").toArray();
        for(int i = 0; i< subArray.size(); i++)
        {
            QJsonObject subObject = subArray.at(i).toObject();
            auto botwebhook = new BotWebhook(&subObject);
            qDebug() << *botwebhook;
        }
    }
}

void BotNetworkManager::on_CreateWebHook(BotNetworkReplyHelper *nrh)
{
    QJsonObject rootObj = this->ExtractContect(nrh);

    auto botwebhook = new BotWebhook(&rootObj);
    qDebug() << *botwebhook;
}

void BotNetworkManager::on_GetWebhookDetails(BotNetworkReplyHelper *nrh)
{
    QJsonObject rootObj = this->ExtractContect(nrh);

    auto botwebhook = new BotWebhook(&rootObj);
    qDebug() << *botwebhook;
}

void BotNetworkManager::on_UpdateWebhook(BotNetworkReplyHelper *nrh)
{
    QJsonObject rootObj = this->ExtractContect(nrh);

    auto botwebhook = new BotWebhook(&rootObj);
    qDebug() << *botwebhook;
}

void BotNetworkManager::on_DeleteWebhook(BotNetworkReplyHelper *nrh)
{
    this->ExtractContect(nrh);
}

void BotNetworkManager::SetHeaderAuthorization(std::shared_ptr<QNetworkRequest> request)
{
    auto botAccessToken = CONFIG->Value(BotConfig::AccessToken).toByteArray();
    request->setRawHeader("Authorization", botAccessToken);
}

std::shared_ptr<QNetworkRequest> BotNetworkManager::NewRequest(const QUrl &url)
{
    std::shared_ptr<QNetworkRequest> request(new QNetworkRequest(url));
    SetHeaderAuthorization(request);
    return request;
}

void BotNetworkManager::SendAndConnect(BotNetworkManager::SendType type,
                                       std::shared_ptr<QNetworkRequest> request,
                                       void (BotNetworkManager::*fuc)(BotNetworkReplyHelper *),
                                       const QJsonObject &jsonObject)
{
    QNetworkReply * reply = nullptr;
    switch(type){
    case SendType::Get:
        reply = networkAccessManager->get(*request);
        break;
    case SendType::Post:
        reply = networkAccessManager->post(*request, QJsonDocument(jsonObject).toJson());
        break;
    case SendType::Put:
        reply = networkAccessManager->put(*request, QJsonDocument(jsonObject).toJson());
        break;
    case SendType::Delete:
        reply = networkAccessManager->deleteResource(*request);
        break;
    }
    if(reply)
    {
        BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
        connect(replyHelper, &BotNetworkReplyHelper::finished, this, fuc);
    }
}


QJsonObject BotNetworkManager::ExtractContect(BotNetworkReplyHelper *nrh)
{
    auto reply = nrh->GetNetworkReply();
    delete nrh;

    QByteArray data = reply->readAll();
    qDebug() << reply->error() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply->deleteLater();

    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(data, &jsonError));

//    if(jsonError.error != QJsonParseError::NoError)
//    {
//        qDebug() << "json error!";
//        return;
//    }
    return jsonDoc.object();
}
