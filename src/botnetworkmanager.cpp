#include "botnetworkmanager.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "botconfig.h"

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
    if(replyGetRooms)
        return;

    QNetworkRequest request;
    request.setUrl(QUrl("https://api.ciscospark.com/v1/rooms"));
    SetHeaderAuthorization(request);

    replyGetRooms = networkAccessManager->get(request);
    connect(replyGetRooms, &QNetworkReply::finished, this, &BotNetworkManager::on_GetRooms);
}

void BotNetworkManager::on_GetRooms()
{
    QByteArray data = replyGetRooms->readAll();

    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(data, &jsonError));

    if(jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!";
        return;
    }

    QJsonObject rootObj = jsonDoc.object();

    QStringList keys = rootObj.keys();
    for(int i = 0; i < keys.size(); i++)
    {
        qDebug() << "key" << i << " is:" << keys.at(i);
    }

    if(rootObj.contains("items"))
    {
        QJsonArray subArray = rootObj.value("items").toArray();
        for(int i = 0; i< subArray.size(); i++)
        {
            QJsonObject subObject = subArray.at(i).toObject();
            qDebug() << "id is:" << subObject.value("id").toString();
            qDebug() << "title is:" << subObject.value("title").toString();
            qDebug() << "type is:" << subObject.value("type").toString();
            qDebug() << "isLocked is:" << subObject.value("isLocked").toString();
            qDebug() << "lastActivity is:" << subObject.value("lastActivity").toString();
            qDebug() << "creatorId is:" << subObject.value("creatorId").toString();
            qDebug() << "created is:" << subObject.value("created").toString();
        }
    }

    replyGetRooms->deleteLater();
    replyGetRooms = nullptr;
}

void BotNetworkManager::SetHeaderAuthorization(QNetworkRequest &request)
{
    auto botAccessToken = CONFIG->Value(BotConfig::AccessToken).toByteArray();
    request.setRawHeader(QByteArray("Authorization"),botAccessToken);
}
