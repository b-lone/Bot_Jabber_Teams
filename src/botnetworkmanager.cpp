#include "botnetworkmanager.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include "botconfig.h"
#include "botroom.h"
#include "botmembership.h"

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

void BotNetworkManager::sendGetMemberships(QString *queryParameters)
{
    QString url = QString("https://api.ciscospark.com/v1/memberships") + queryParameters;
    auto request = this->NewRequest(QUrl(url));

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

    QJsonDocument jsonDoc(jsonObject);

    qDebug() << jsonDoc.toJson();
    QNetworkReply * reply = networkAccessManager->post(*(request.get()), jsonDoc.toJson());
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_CreateAMembership);
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
