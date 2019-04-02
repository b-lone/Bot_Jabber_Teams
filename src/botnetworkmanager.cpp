#include "botnetworkmanager.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QHttpMultiPart>
#include <QFile>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QMetaEnum>
#include "botconfig.h"
#include "botroom.h"
#include "botmembership.h"
#include "botmessage.h"
#include "botpeople.h"
#include "botwebhook.h"
#include "botcommon.h"
#include "botstore.h"

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

    auto botAccessToken = BOTCONFIG->Value(BotConfig::BotAccessToken).toByteArray();
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
    BOTLOG(sUrl);
    request->setUrl(sUrl);
    return request;
}

void BotNetworkRepquestHelper::setContentType(const QString &contentType)
{
    this->request->setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, contentType);
}

QString BotNetworkRepquestHelper::RequestTypeToString(BotNetworkRepquestHelper::RequestType type)
{
    return  QMetaEnum::fromType<RequestType>().valueToKey(type);
//    static QMap<RequestType, QString> queryList = {
//        {RequestType::memberships, "memberships"},
//        {RequestType::messages, "messages"},
//        {RequestType::people, "people"},
//        {RequestType::rooms, "rooms"},
//        {RequestType::webhooks, "webhooks"}
//    };
//    return queryList[type];
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
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::memberships);
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
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::memberships);
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

    SendAndConnect(SendType::Post, request, &BotNetworkManager::on_CreateAMembership, QJsonDocument(jsonObject).toJson());
}

void BotNetworkManager::sendGetMembership(QString membershipId)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::memberships);
    networkRequestHelper.setObjectId(membershipId);
    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Get, request, &BotNetworkManager::on_GetMembership);
}

void BotNetworkManager::sendUpdateMembership(QString membershipId, bool isModerator)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::memberships);
    networkRequestHelper.setObjectId(membershipId);
    networkRequestHelper.setContentType(BotNetworkRepquestHelper::contentType_Json);
    auto request = networkRequestHelper.GennerateRequest();

    QJsonObject jsonObject;
    jsonObject.insert("isModerator", isModerator);

    SendAndConnect(SendType::Put, request, &BotNetworkManager::on_UpdateMembership, QJsonDocument(jsonObject).toJson());
}

void BotNetworkManager::sendDeleteMembership(QString membershipId)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::memberships);
    networkRequestHelper.setObjectId(membershipId);
    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Delete, request, &BotNetworkManager::on_DeleteMembership);
}

void BotNetworkManager::sendListMessages(QString roomId, QString mentionedPeople, QString before, QString beforeMessage, int max)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::messages);
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
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::messages);
    networkRequestHelper.setObjectId("direct");
    if(isByEmail){
        networkRequestHelper.setParams("personEmail", byString);
    }else {
        networkRequestHelper.setParams("personId", byString);
    }
    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Get, request, &BotNetworkManager::on_ListDirectMessages);
}

void BotNetworkManager::sendCreateMessage(const BotMessage &message)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::messages);
    auto request = networkRequestHelper.GennerateRequest();

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

    SendAndConnect(SendType::Post, request, &BotNetworkManager::on_CreateMessage, multiPart);
}

void BotNetworkManager::sendGetMessageDetails(QString messageId)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::messages);
    networkRequestHelper.setObjectId(messageId);
    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Get, request, &BotNetworkManager::on_GetMessageDetails);
}

void BotNetworkManager::sendDeleteMessage(QString messageId)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::messages);
    networkRequestHelper.setObjectId(messageId);
    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Delete, request, &BotNetworkManager::on_DeleteMessage);
}

void BotNetworkManager::sendListPeople(QString byString, int byWhat, int max)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::people);
    switch(byWhat){
    case 0:
        networkRequestHelper.setParams("email", byString);
        break;
    case 1:
        networkRequestHelper.setParams("displayName", byString);
        break;
    case 2:
        networkRequestHelper.setParams("id", byString);
        break;
    default:
        BOTLOG("Error:Unexcepted option!");
        return;
    }
    networkRequestHelper.setParams("max", QString::number(max));
    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Get, request, &BotNetworkManager::on_ListPeople);
}

void BotNetworkManager::sendGetPersonDetails(QString personId)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::people);
    networkRequestHelper.setObjectId(personId);
    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Get, request, &BotNetworkManager::on_GetPersonDetails);
}

void BotNetworkManager::sendListRooms(QString type, QString sortBy, int max)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::rooms);
    if(type != ""){
        networkRequestHelper.setParams("type", type);
    }
    if(sortBy != ""){
        networkRequestHelper.setParams("sortBy", sortBy);
    }
    networkRequestHelper.setParams("max", QString::number(max));
    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Get, request, &BotNetworkManager::on_ListRooms);
}

void BotNetworkManager::sendCreateRoom(QString title)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::rooms);
    networkRequestHelper.setContentType(BotNetworkRepquestHelper::contentType_Json);
    auto request = networkRequestHelper.GennerateRequest();

    QJsonObject jsonObject;
    jsonObject.insert("title", title);

    SendAndConnect(SendType::Post, request, &BotNetworkManager::on_CreateRoom, QJsonDocument(jsonObject).toJson());
}

void BotNetworkManager::sendGetRoomDetails(QString roomId)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::rooms);
    networkRequestHelper.setObjectId(roomId);
    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Delete, request, &BotNetworkManager::on_GetRoomDetails);
}

void BotNetworkManager::sendUpdateRoom(QString roomId, QString title)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::rooms);
    networkRequestHelper.setContentType(BotNetworkRepquestHelper::contentType_Json);
    networkRequestHelper.setObjectId(roomId);
    auto request = networkRequestHelper.GennerateRequest();

    QJsonObject jsonObject;
    jsonObject.insert("title", title);

    SendAndConnect(SendType::Put, request, &BotNetworkManager::on_UpdateRoom, QJsonDocument(jsonObject).toJson());
}

void BotNetworkManager::sendDeleteRoom(QString roomId)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::rooms);
    networkRequestHelper.setObjectId(roomId);
    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Delete, request, &BotNetworkManager::on_DeleteRoom);
}

void BotNetworkManager::sendListWebhooks(int max)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::webhooks);
    networkRequestHelper.setParams("max", QString::number(max));
    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Get, request, &BotNetworkManager::on_ListWebhooks);
}

void BotNetworkManager::sendCreateWebhook(BotWebhook & webhook)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::webhooks);
    networkRequestHelper.setContentType(BotNetworkRepquestHelper::contentType_Json);
    auto request = networkRequestHelper.GennerateRequest();

    auto jsonObjectPtr = webhook.Wrap();

    SendAndConnect(SendType::Post, request, &BotNetworkManager::on_CreateWebHook, QJsonDocument(*jsonObjectPtr).toJson());
}

void BotNetworkManager::sendGetWebhookDetails(QString webhookId)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::webhooks);
    networkRequestHelper.setObjectId(webhookId);
    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Get, request, &BotNetworkManager::on_GetWebhookDetails);
}

void BotNetworkManager::sendUpdateWebhook(QString webhookId, QString name, QString targetUrl, QString status)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::webhooks);
    networkRequestHelper.setContentType(BotNetworkRepquestHelper::contentType_Json);
    networkRequestHelper.setObjectId(webhookId);
    auto request = networkRequestHelper.GennerateRequest();

    QJsonObject jsonObject;
    jsonObject.insert("name", name);
    jsonObject.insert("targetUrl", targetUrl);
    jsonObject.insert("status", status);

    SendAndConnect(SendType::Put, request, &BotNetworkManager::on_UpdateWebhook, QJsonDocument(jsonObject).toJson());
}

void BotNetworkManager::sendDeleteWebhook(QString webhookId)
{
    BotNetworkRepquestHelper networkRequestHelper(BotNetworkRepquestHelper::RequestType::rooms);
    networkRequestHelper.setObjectId(webhookId);
    auto request = networkRequestHelper.GennerateRequest();

    SendAndConnect(SendType::Delete, request, &BotNetworkManager::on_DeleteWebhook);
}

void BotNetworkManager::on_GetNgrokInfo(BotNetworkReplyHelper *nrh)
{
    auto rootObj = this->ExtractContect(nrh);
    BOTLOG(* rootObj);
    if(rootObj->contains("uri")){
        BOTLOG(rootObj->value("uri").toString());
    }else {
        BOTLOG("Error in get ngrok info!");
        return;
    }
//    BOTLOG(*rootObj);
    {
        auto tunnelsObj = rootObj->value("tunnels").toArray();
        for (auto tunnel: tunnelsObj) {
            auto tunnelObj = tunnel.toObject();
            if(tunnelObj.value("proto").toString() == "https"){
                auto sURl = tunnelObj.value("public_url").toString();

                BOTLOG("Creat new webhook:" << sURl);
                BotWebhook webhook;
                webhook.name = "Kun You";
                webhook.targetUrl = sURl;
                webhook.resource = "messages";
                webhook.event = "created";

                this->sendCreateWebhook(webhook);
            }
        }
    }
}

void BotNetworkManager::on_GetMemberships(BotNetworkReplyHelper *nrh)
{
    auto rootObj = this->ExtractContect(nrh);

    if(rootObj){
        if(rootObj->contains("items")){
            QJsonArray subArray = rootObj->value("items").toArray();
            for(int i = 0; i< subArray.size(); i++)
            {
                QJsonObject subObject = subArray.at(i).toObject();
                auto botmembership = new BotMembership(&subObject);
                BOTLOG(*botmembership);
            }
        }
    }

}

void BotNetworkManager::on_CreateAMembership(BotNetworkReplyHelper *nrh)
{
    auto rootObj = this->ExtractContect(nrh);

    if(rootObj){
        auto botmembership = new BotMembership(rootObj.get());
        BOTLOG(*botmembership);
    }
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
    auto rootObj = this->ExtractContect(nrh);

    if(rootObj){
        if(rootObj->contains("items")){
            QJsonArray subArray = rootObj->value("items").toArray();
            for(int i = 0; i< subArray.size(); i++)
            {
                QJsonObject subObject = subArray.at(i).toObject();
                auto botmessage = new BotMessage(&subObject);
                BOTLOG(*botmessage);
            }
        }
    }
}

void BotNetworkManager::on_ListDirectMessages(BotNetworkReplyHelper *nrh)
{
    on_ListMessages(nrh);
}

void BotNetworkManager::on_CreateMessage(BotNetworkReplyHelper *nrh)
{
    auto rootObj = this->ExtractContect(nrh);

    if(rootObj){
        auto botmessage= new BotMessage(rootObj.get());
        BOTLOG(*botmessage);
    }
}

void BotNetworkManager::on_GetMessageDetails(BotNetworkReplyHelper *nrh)
{
    auto rootObj = this->ExtractContect(nrh);

    if(rootObj){
        std::shared_ptr<BotMessage> botmessage(new BotMessage(rootObj.get()));
        BOTLOG(*botmessage);
        BOTSTORE->PushMessage(botmessage);
    }
}

void BotNetworkManager::on_DeleteMessage(BotNetworkReplyHelper *nrh)
{
    this->ExtractContect(nrh);
}

void BotNetworkManager::on_ListPeople(BotNetworkReplyHelper *nrh)
{
    auto rootObj = this->ExtractContect(nrh);

    if(rootObj){
        if(rootObj->contains("items")){
            QJsonArray subArray = rootObj->value("items").toArray();
            for(int i = 0; i< subArray.size(); i++)
            {
                QJsonObject subObject = subArray.at(i).toObject();
                auto botpeople = new BotPeople(&subObject);
                BOTLOG(*botpeople);
            }
        }
    }
}

void BotNetworkManager::on_GetPersonDetails(BotNetworkReplyHelper *nrh)
{
    auto rootObj = this->ExtractContect(nrh);

    if(rootObj){
        auto botpeople = new BotPeople(rootObj.get());
        BOTLOG(*botpeople);
    }
}

void BotNetworkManager::on_ListRooms(BotNetworkReplyHelper *nrh)
{
    auto rootObj = this->ExtractContect(nrh);

    if(rootObj->contains("items"))
    {
        QJsonArray subArray = rootObj->value("items").toArray();
        for(int i = 0; i< subArray.size(); i++)
        {
            QJsonObject subObject = subArray.at(i).toObject();
            auto botroom = new BotRoom(&subObject);
            qDebug() << *botroom;
        }
    }
}

void BotNetworkManager::on_CreateRoom(BotNetworkReplyHelper *nrh)
{
    on_GetRoomDetails(nrh);
}

void BotNetworkManager::on_GetRoomDetails(BotNetworkReplyHelper *nrh)
{
    auto rootObj = this->ExtractContect(nrh);

    auto botroom = new BotRoom(rootObj.get());
    qDebug() << *botroom;
}

void BotNetworkManager::on_UpdateRoom(BotNetworkReplyHelper *nrh)
{
    on_GetRoomDetails(nrh);
}

void BotNetworkManager::on_DeleteRoom(BotNetworkReplyHelper *nrh)
{
   this->ExtractContect(nrh);
}

void BotNetworkManager::on_ListWebhooks(BotNetworkReplyHelper *nrh)
{
    auto rootObj = this->ExtractContect(nrh);

    if(rootObj){
        if(rootObj->contains("items")){
            QJsonArray subArray = rootObj->value("items").toArray();
            for(int i = 0; i< subArray.size(); i++)
            {
                QJsonObject subObject = subArray.at(i).toObject();
                auto botwebhook = new BotWebhook(&subObject);
                BOTLOG(*botwebhook);
            }
        }
    }
}

void BotNetworkManager::on_CreateWebHook(BotNetworkReplyHelper *nrh)
{
    auto rootObj = this->ExtractContect(nrh);

    if(rootObj){
        std::shared_ptr<BotWebhook> botwebhook(new BotWebhook(rootObj.get()));
        BOTLOG(*botwebhook);
        BOTSTORE->PushWebhook(botwebhook);
    }
}

void BotNetworkManager::on_GetWebhookDetails(BotNetworkReplyHelper *nrh)
{
    auto rootObj = this->ExtractContect(nrh);

    if(rootObj){
        auto botwebhook = new BotWebhook(rootObj.get());
        BOTLOG(*botwebhook);
    }
}

void BotNetworkManager::on_UpdateWebhook(BotNetworkReplyHelper *nrh)
{
    auto rootObj = this->ExtractContect(nrh);

    if(rootObj){
        auto botwebhook = new BotWebhook(rootObj.get());
        BOTLOG(*botwebhook);
    }
}

void BotNetworkManager::on_DeleteWebhook(BotNetworkReplyHelper *nrh)
{
    this->ExtractContect(nrh);
}

void BotNetworkManager::SendAndConnect(BotNetworkManager::SendType type,
                                       std::shared_ptr<QNetworkRequest> request,
                                       void (BotNetworkManager::*fuc)(BotNetworkReplyHelper *),
                                       const QByteArray &data)
{
    BOTLOG("Sending Begin!");
    QNetworkReply * reply = nullptr;
    switch(type){
    case SendType::Get:
        reply = networkAccessManager->get(*request);
        break;
    case SendType::Post:
        reply = networkAccessManager->post(*request, data);
        break;
    case SendType::Put:
        reply = networkAccessManager->put(*request, data);
        break;
    case SendType::Delete:
        reply = networkAccessManager->deleteResource(*request);
        break;
    }
    if(reply)
    {
        BOTLOG("Sending Succeed!");
        BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
        connect(replyHelper, &BotNetworkReplyHelper::finished, this, fuc);
    }else {
        BOTLOG("Sending failed!");
    }
}

void BotNetworkManager::SendAndConnect(BotNetworkManager::SendType type,
                                       std::shared_ptr<QNetworkRequest> request,
                                       void (BotNetworkManager::*fuc)(BotNetworkReplyHelper *),
                                       QHttpMultiPart *data)
{
    BOTLOG("Sending Begin!");
    QNetworkReply * reply = nullptr;
    switch(type){
    case SendType::Get:
        reply = networkAccessManager->get(*request);
        break;
    case SendType::Post:
        reply = networkAccessManager->post(*request, data);
        break;
    case SendType::Put:
        reply = networkAccessManager->put(*request, data);
        break;
    case SendType::Delete:
        reply = networkAccessManager->deleteResource(*request);
        break;
    }
    if(reply)
    {
        BOTLOG("Sending Succeed!");
        if(data){
            data->setParent(reply);
        }

        BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
        connect(replyHelper, &BotNetworkReplyHelper::finished, this, fuc);
    }else {
        BOTLOG("Sending failed!");
    }
}


std::shared_ptr<QJsonObject> BotNetworkManager::ExtractContect(BotNetworkReplyHelper *nrh)
{
    auto reply = nrh->GetNetworkReply();

    auto errorCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    BOTLOG("Reply status:(" + QString::number(errorCode) + "):" << reply->error());

    QByteArray data = reply->readAll();
    reply->deleteLater();

    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(data, &jsonError));
    BOTLOG(QString("Json parsing result:") << jsonError.error);
    if(jsonError.error != QJsonParseError::ParseError::NoError){
        return {};
    }

    if(errorCode == 200){
        std::shared_ptr<QJsonObject> jsonObjectPtr(new QJsonObject);
        *jsonObjectPtr = jsonDoc.object();
        return jsonObjectPtr;
    } else if (errorCode == 204) {
        return {};
    } else {
        QJsonObject jsonObject = jsonDoc.object();
        if(jsonObject.contains("message"))
            BOTLOG(jsonObject.value("message"));      
    }
    return {};
}
