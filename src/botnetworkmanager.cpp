#include "botnetworkmanager.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "botconfig.h"
#include "botroom.h"

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
    QNetworkRequest request;
    request.setUrl(QUrl("https://api.ciscospark.com/v1/rooms"));
    SetHeaderAuthorization(request);

    QNetworkReply * reply = networkAccessManager->get(request);
    BotNetworkReplyHelper * replyHelper = new BotNetworkReplyHelper(reply);
    connect(replyHelper, &BotNetworkReplyHelper::finished, this, &BotNetworkManager::on_GetRooms);
}

void BotNetworkManager::on_GetRooms(BotNetworkReplyHelper * nrh)
{
    auto reply = nrh->GetNetworkReply();
    delete nrh;

    QByteArray data = reply->readAll();
    reply->deleteLater();

    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(data, &jsonError));

    if(jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!";
        return;
    }

    QJsonObject rootObj = jsonDoc.object();

//    QStringList keys = rootObj.keys();
//    for(int i = 0; i < keys.size(); i++)
//    {
//        qDebug() << "key" << i << " is:" << keys.at(i);
//    }

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

void BotNetworkManager::SetHeaderAuthorization(QNetworkRequest &request)
{
    auto botAccessToken = CONFIG->Value(BotConfig::AccessToken).toByteArray();
    request.setRawHeader(QByteArray("Authorization"),botAccessToken);
}
