#include "botnetworkmanager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QMetaEnum>

#include "botconfig.h"
#include "botwebhook.h"
#include "botmessage.h"
#include "botcommon.h"
#include "botnetworkcontroller.h"


//BotNetworkManager
BotNetworkManager::BotNetworkManager()
{
    networkAccessManager = std::make_shared<QNetworkAccessManager>(this);

    networkController = new BotNetworkController(this);
    connect(this, &BotNetworkManager::dataReady, networkController, &BotNetworkController::on_dataReady);
    connect(this, &BotNetworkManager::ngrokReady, networkController, &BotNetworkController::on_ngrokReady);
}

BotNetworkManager *BotNetworkManager::Instance()
{
    static BotNetworkManager * botNetworkManager = new BotNetworkManager();
    return botNetworkManager;
}

void BotNetworkManager::sendGetNgrokInfo()
{
    QNetworkRequest request(QUrl("http://localhost:4040/api/tunnels"));
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader,"application/json");

    auto reply = networkAccessManager->get(request);
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_GetNgrokInfo);

}

void BotNetworkManager::sendGetMemberships(QString roomId, QString filterString, bool isFilterByPersonId)
{
    auto networkRequestHelper = BotNetworkRepquestHelper::New(RequestType::memberships, SendType::Get);
    if(roomId != ""){
        networkRequestHelper->setParams("roomId",roomId);
        if(filterString != ""){
            if(isFilterByPersonId){
                networkRequestHelper->setParams("personId", filterString);
            }else {
                networkRequestHelper->setParams("personEmail", filterString);
            }
        }
    }

    SendAndConnect(networkRequestHelper);
}

void BotNetworkManager::sendCreateAMembership(QString roomId, bool isModerator, QString byString, bool isByEmail)
{
    auto networkRequestHelper = BotNetworkRepquestHelper::New(RequestType::memberships, SendType::Post);
    networkRequestHelper->setContentType(BotNetworkRepquestHelper::contentType_Json);

    QJsonObject jsonObject;
    jsonObject.insert("roomId", roomId);
    jsonObject.insert("isModerator", isModerator);
    if(isByEmail){
        jsonObject.insert("personEmail", byString);
    }else {
        jsonObject.insert("personId", byString);
    }
    networkRequestHelper->setBodyByte(jsonObject);

    SendAndConnect(networkRequestHelper);
}

void BotNetworkManager::sendUpdateMembership(QString membershipId, bool isModerator)
{
    auto networkRequestHelper = BotNetworkRepquestHelper::New(RequestType::memberships, SendType::Put);
    networkRequestHelper->setObjectId(membershipId);
    networkRequestHelper->setContentType(BotNetworkRepquestHelper::contentType_Json);

    QJsonObject jsonObject;
    jsonObject.insert("isModerator", isModerator);
    networkRequestHelper->setBodyByte(jsonObject);

    SendAndConnect(networkRequestHelper);
}

void BotNetworkManager::sendListMessages(QString roomId, QString mentionedPeople, QString before, QString beforeMessage, int max)
{
    auto networkRequestHelper = BotNetworkRepquestHelper::New(RequestType::messages, SendType::Get);
    networkRequestHelper->setParams("roomId", roomId);
    if(mentionedPeople != ""){
        networkRequestHelper->setParams("mentionedPeople", mentionedPeople);
    }
    if(before != ""){
        networkRequestHelper->setParams("before", before);
    }
    if(beforeMessage != ""){
        networkRequestHelper->setParams("beforeMessage", beforeMessage);
    }
    networkRequestHelper->setParams("max", QString::number(max));

    SendAndConnect(networkRequestHelper);
}

void BotNetworkManager::sendListDirectMessages(QString byString, bool isByEmail)
{
    auto networkRequestHelper = BotNetworkRepquestHelper::New(RequestType::messages, SendType::Get);
    networkRequestHelper->setObjectId("direct");
    if(isByEmail){
        networkRequestHelper->setParams("personEmail", byString);
    }else {
        networkRequestHelper->setParams("personId", byString);
    }
    SendAndConnect(networkRequestHelper);
}

void BotNetworkManager::sendCreateMessage(const BotMessage &message)
{
    auto networkRequestHelper = BotNetworkRepquestHelper::New(RequestType::messages, SendType::Post);

    QHttpMultiPart * multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    if(message.roomId != ""){
        BOTLOG("Create by roomId.");
        QHttpPart roomIdPart;
        roomIdPart.setHeader(QNetworkRequest::KnownHeaders::ContentDispositionHeader, "form-data; name=\"roomId\"");
        roomIdPart.setBody(message.roomId.toUtf8());
        multiPart->append(roomIdPart);
    }else if (message.toPersonId != "") {
        BOTLOG("Create by personId.");
        QHttpPart toPersonIdPart;
        toPersonIdPart.setHeader(QNetworkRequest::KnownHeaders::ContentDispositionHeader, "form-data; name=\"toPersonId\"");
        toPersonIdPart.setBody(message.toPersonId.toUtf8());
        multiPart->append(toPersonIdPart);
    }else if (message.toPersonEmail != "") {
        BOTLOG("Create by personEmail.");
        QHttpPart toPersonEmailPart;
        toPersonEmailPart.setHeader(QNetworkRequest::KnownHeaders::ContentDispositionHeader, "form-data; name=\"toPersonEmail\"");
        toPersonEmailPart.setBody(message.toPersonEmail.toUtf8());
        multiPart->append(toPersonEmailPart);
    }else {
        BOTLOG("No destination!");
        return;
    }

    bool ifHaveContent = false;
    if(message.text != "") {
        QHttpPart textPart;
        textPart.setHeader(QNetworkRequest::KnownHeaders::ContentDispositionHeader, "form-data; name=\"text\"");
        textPart.setBody(message.text.toUtf8());
        multiPart->append(textPart);

        ifHaveContent = true;
    }
    if(message.markdown != "") {
        QHttpPart markdownPart;
        markdownPart.setHeader(QNetworkRequest::KnownHeaders::ContentDispositionHeader, "form-data; name=\"markdown\"");
        markdownPart.setBody(message.markdown.toUtf8());
        multiPart->append(markdownPart);

        ifHaveContent = true;
    }
    if(message.files.count()) {
        QHttpPart filesPart;
        QString filePath = message.files[0];

        QFile * file = new QFile(filePath);
        if(file->open(QIODevice::ReadOnly)){
            filesPart.setBodyDevice(file);
            file->setParent(multiPart);
        }else {
            BOTLOG("File open failed!");
            delete multiPart;
            delete file;
            return;
        }

        QMimeType type = QMimeDatabase().mimeTypeForFile(filePath);
        BOTLOG(type.name());
        filesPart.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, type.name());
        filesPart.setHeader(QNetworkRequest::KnownHeaders::ContentDispositionHeader, "form-data; name=\"files\" ;filename =" + file->fileName());

        multiPart->append(filesPart);

        ifHaveContent = true;
    }

    if(!ifHaveContent){
        BOTLOG("Error:No Content!");
        return;
    }

    networkRequestHelper->setBodyMutiPart(multiPart);

    SendAndConnect(networkRequestHelper);
}

void BotNetworkManager::sendListPeople(QString byString, int byWhat, int max)
{
    auto networkRequestHelper = BotNetworkRepquestHelper::New(RequestType::people, SendType::Get);
    switch(byWhat){
    case 0:
        networkRequestHelper->setParams("email", byString);
        break;
    case 1:
        networkRequestHelper->setParams("displayName", byString);
        break;
    case 2:
        networkRequestHelper->setParams("id", byString);
        break;
    default:
        BOTLOG("Error:Unexcepted option!");
        return;
    }
    networkRequestHelper->setParams("max", QString::number(max));


    SendAndConnect(networkRequestHelper);
}

void BotNetworkManager::sendListRooms(QString type, QString sortBy, int max)
{
    auto networkRequestHelper = BotNetworkRepquestHelper::New(RequestType::rooms, SendType::Get);
    if(type != ""){
        networkRequestHelper->setParams("type", type);
    }
    if(sortBy != ""){
        networkRequestHelper->setParams("sortBy", sortBy);
    }
    networkRequestHelper->setParams("max", QString::number(max));


    SendAndConnect(networkRequestHelper);
}

void BotNetworkManager::sendCreateRoom(QString title)
{
    auto networkRequestHelper = BotNetworkRepquestHelper::New(RequestType::rooms, SendType::Post);
    networkRequestHelper->setContentType(BotNetworkRepquestHelper::contentType_Json);

    QJsonObject jsonObject;
    jsonObject.insert("title", title);
    networkRequestHelper->setBodyByte(jsonObject);

    SendAndConnect(networkRequestHelper);
}

void BotNetworkManager::sendUpdateRoom(QString roomId, QString title)
{
    auto networkRequestHelper = BotNetworkRepquestHelper::New(RequestType::rooms, SendType::Put);
    networkRequestHelper->setContentType(BotNetworkRepquestHelper::contentType_Json);
    networkRequestHelper->setObjectId(roomId);

    QJsonObject jsonObject;
    jsonObject.insert("title", title);
    networkRequestHelper->setBodyByte(jsonObject);

    SendAndConnect(networkRequestHelper);
}

void BotNetworkManager::sendListWebhooks(int max)
{
    auto networkRequestHelper = BotNetworkRepquestHelper::New(RequestType::webhooks, SendType::Get);
    networkRequestHelper->setParams("max", QString::number(max));

    SendAndConnect(networkRequestHelper);
}

void BotNetworkManager::sendCreateWebhook(BotWebhook & webhook)
{
    auto networkRequestHelper = BotNetworkRepquestHelper::New(RequestType::webhooks, SendType::Post);
    networkRequestHelper->setContentType(BotNetworkRepquestHelper::contentType_Json);

    auto jsonObjectPtr = webhook.Wrap();
    networkRequestHelper->setBodyByte(*jsonObjectPtr);

    SendAndConnect(networkRequestHelper);
}

void BotNetworkManager::sendUpdateWebhook(QString webhookId, QString name, QString targetUrl, QString status)
{
    auto networkRequestHelper = BotNetworkRepquestHelper::New(RequestType::webhooks, SendType::Put);
    networkRequestHelper->setContentType(BotNetworkRepquestHelper::contentType_Json);
    networkRequestHelper->setObjectId(webhookId);

    QJsonObject jsonObject;
    jsonObject.insert("name", name);
    jsonObject.insert("targetUrl", targetUrl);
    jsonObject.insert("status", status);

    SendAndConnect(networkRequestHelper);
}

void BotNetworkManager::sendGetDetails(RequestType rt, QString id)
{
    auto networkRequestHelper = BotNetworkRepquestHelper::New(rt, SendType::Get);
    networkRequestHelper->setObjectId(id);

    SendAndConnect(networkRequestHelper);
}

void BotNetworkManager::sendDelete(RequestType rt, QString id)
{
    auto networkRequestHelper = BotNetworkRepquestHelper::New(rt, SendType::Delete);
    networkRequestHelper->setObjectId(id);

    SendAndConnect(networkRequestHelper);
}

void BotNetworkManager::on_GetNgrokInfo(BotNetworkReplyHelper *nrh)
{
    BOTLOG("Receive message from ngrok");
    auto reply = nrh->GetNetworkReply();

    auto errorCode = ParseReplyResult(reply);
    if(errorCode < 200 || errorCode >= 300){
        return;
    }
    std::shared_ptr<QByteArray> data(new QByteArray(reply->readAll()));
    emit ngrokReady(data);
    reply->deleteLater();
}

void BotNetworkManager::on_finished(BotNetworkReplyHelper *nrh)
{
    auto reply = nrh->GetNetworkReply();
    auto result = ParseReplyResult(reply);
    if (result == 200) {
        std::shared_ptr<QByteArray> data(new QByteArray(reply->readAll()));
        emit dataReady(data, nrh->getRequestType());
    }else if (result == 204) {
        emit readyWithoutData(nrh->getRequestType());
    }
    reply->deleteLater();
}

void BotNetworkManager::SendAndConnect(std::shared_ptr<BotNetworkRepquestHelper> requestHelper)
{
    BOTLOG("Sending Begin!");
    QNetworkReply * reply = nullptr;
    auto request = requestHelper->GenerateRequest();
    switch(requestHelper->getSendType()){
    case SendType::Get:
        reply = networkAccessManager->get(*request);
        break;
    case SendType::Post:
        if(requestHelper->getContentType() == BotNetworkRepquestHelper::contentType_Json){
            reply = networkAccessManager->post(*request, requestHelper->getBodyByte());
        }else {
            reply = networkAccessManager->post(*request, requestHelper->getBodyMutiPart());
        }
        break;
    case SendType::Put:
        if(requestHelper->getContentType() == BotNetworkRepquestHelper::contentType_Json){
            reply = networkAccessManager->post(*request, requestHelper->getBodyByte());
        }else {
            auto body = requestHelper->getBodyMutiPart();
            reply = networkAccessManager->post(*request, body);
            body->setParent(reply);
        }
        break;
    case SendType::Delete:
        reply = networkAccessManager->deleteResource(*request);
        break;
    }
    if(reply)
    {
        BOTLOG("Sending Succeed!");

        BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
        replyHelper->setRequestType(requestHelper->getRequestType());
        connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_finished);
    }else {
        BOTLOG("Sending failed!");
    }
}

int BotNetworkManager::ParseReplyResult(QNetworkReply *reply)
{
    auto errorCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    BOTLOG("Reply status:(" + QString::number(errorCode) + "):" << reply->error());
    if(errorCode < 200 || errorCode >= 300){
        QByteArray data = reply->readAll();
        QJsonObject * jsonObject = new QJsonObject;
        if(ParseBytesToJson(data, jsonObject)){
            if(jsonObject->contains("message"))
                BOTLOG(jsonObject->value("message"));
        }
        delete  jsonObject;
    }

    return errorCode;
}
