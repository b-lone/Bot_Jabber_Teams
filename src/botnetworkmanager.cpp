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

//BotNetworkReplyHelper
BotNetworkReplyHelper::BotNetworkReplyHelper(QNetworkReply *nr):networkReply(nr)
{
    connect(nr, &QNetworkReply::finished, this, &BotNetworkReplyHelper::on_finished);
}

void BotNetworkReplyHelper::on_finished()
{
    emit finished(this);
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

void BotNetworkManager::sendGetRooms()
{
    auto request = this->NewRequest(QUrl("https://api.ciscospark.com/v1/rooms"));

    QNetworkReply * reply = networkAccessManager->get(*(request.get()));
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_GetRooms);
}

void BotNetworkManager::sendGetMemberships(QString queryParameters)
{
    QString surl = QString("https://api.ciscospark.com/v1/memberships") + queryParameters;
    auto request = this->NewRequest(QUrl(surl));

    QNetworkReply * reply = networkAccessManager->get(*(request.get()));
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_GetMemberships);
}

void BotNetworkManager::sendCreateAMembership(QString roomId, QString personEmail, bool isModerator)
{
    auto request = this->NewRequest(QUrl("https://api.ciscospark.com/v1/memberships"));
    request->setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, QByteArray("application/json"));

    QJsonObject jsonObject;
    jsonObject.insert("roomId", roomId);
//    jsonObject.insert("personId", personId);
    jsonObject.insert("personEmail", personEmail);
    jsonObject.insert("isModerator", isModerator);

    QNetworkReply * reply = networkAccessManager->post(*(request.get()), QJsonDocument(jsonObject).toJson());
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_CreateAMembership);
}

void BotNetworkManager::sendGetMembership(QString membershipId)
{
    QString surl = QString("https://api.ciscospark.com/v1/memberships/") + membershipId;
    auto request = this->NewRequest(QUrl(surl));

    QNetworkReply * reply = networkAccessManager->get(*(request.get()));
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_GetMembership);
}

void BotNetworkManager::sendUpdateMembership(QString membershipId, bool isModerator)
{
    QString surl = QString("https://api.ciscospark.com/v1/memberships/") + membershipId;
    auto request = this->NewRequest(QUrl(surl));
    request->setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

    QJsonObject jsonObject;
    jsonObject.insert("isModerator", isModerator);

    QNetworkReply * reply = networkAccessManager->put(*(request.get()), QJsonDocument(jsonObject).toJson());
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_UpdateMembership);
}

void BotNetworkManager::sendDeleteMembership(QString membershipId)
{
    QString surl = QString("https://api.ciscospark.com/v1/memberships/") + membershipId;
    auto request = this->NewRequest(QUrl(surl));

    QNetworkReply * reply = networkAccessManager->deleteResource(*(request.get()));
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_DeleteMembership);
}

void BotNetworkManager::sendListMessages(QString roomId)
{
    QString surl = QString("https://api.ciscospark.com/v1/messages?roomId=") + roomId;
    auto request = this->NewRequest(QUrl(surl));

    QNetworkReply * reply = networkAccessManager->get(*(request.get()));
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_ListMessages);
}

void BotNetworkManager::sendListDirectMessages(QString personEmail)
{
    QString surl = QString("https://api.ciscospark.com/v1/messages/direct?personEmail=") + personEmail;
    auto request = this->NewRequest(QUrl(surl));

    QNetworkReply * reply = networkAccessManager->get(*(request.get()));
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_ListDirectMessages);
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

void BotNetworkManager::on_GetRooms(BotNetworkReplyHelper * nrh)
{
    QJsonObject rootObj = this->ExtractContect(nrh);

    if(rootObj.contains("items"))
    {
        QJsonArray subArray = rootObj.value("items").toArray();
        for(int i = 0; i< subArray.size(); i++)
        {
            QJsonObject subObject = subArray.at(i).toObject();
            auto botroom = new BotRoom(&subObject);
            qDebug() << *botroom;
        }
    }
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

void BotNetworkManager::SetHeaderAuthorization(std::shared_ptr<QNetworkRequest> request)
{
    auto botAccessToken = CONFIG->Value(BotConfig::AccessToken).toByteArray();
    request->setRawHeader(QByteArray("Authorization"),botAccessToken);
}

std::shared_ptr<QNetworkRequest> BotNetworkManager::NewRequest(const QUrl &url)
{
    std::shared_ptr<QNetworkRequest> request(new QNetworkRequest(url));
    SetHeaderAuthorization(request);
    return request;
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
