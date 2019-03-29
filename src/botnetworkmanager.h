#ifndef BOTNETWORKMANAGER_H
#define BOTNETWORKMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QMap>
#include <QMetaEnum>
class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;
class QHttpMultiPart;
class BotWebhook;
class BotMessage;

class BotNetworkReplyHelper : public QObject
{
    Q_OBJECT
public:
    BotNetworkReplyHelper(QNetworkReply * nr);
    QNetworkReply * GetNetworkReply(){ return networkReply; }

signals:
    void finished(BotNetworkReplyHelper * nrh);

private slots:
    void on_finished();

private:
    QNetworkReply * networkReply;

};

class BotNetworkRepquestHelper : public QObject
{
    Q_OBJECT
public:
    enum RequestType{
        memberships = 0,
        messages,
        people,
        rooms,
        webhooks,
    };
    Q_ENUM(RequestType)

    BotNetworkRepquestHelper(RequestType type);

    std::shared_ptr<QNetworkRequest> GennerateRequest();

    void setParams(const QString &key, const QString &value){ params[key] = value; }

    void setContentType(const QString & contentType);

    void setObjectId(const QString & Id){ objectId = Id; }
private:
    QString RequestTypeToString(RequestType type);
private:
    std::shared_ptr<QNetworkRequest> request;
    QMap<QString, QString> params;
    RequestType requestType;
    QString objectId;
public:
    static const QString contentType_Json;
    static const QString mUrl;
};

class BotNetworkManager : public QObject
{
    Q_OBJECT
public:
    enum SendType{
      Get = 0,
      Post,
      Put,
      Delete,
    };

    static BotNetworkManager * Instance();
    void sendGetNgrokInfo();
//---------------Memberships----------------------------------
    //https://developer.webex.com/docs/api/v1/memberships/list-memberships
    //1.默认查询列出本人加入的room的本人的memberships，即加入几个room就会有几个结果返回
    //2.使用roomId查询某个room内所有成员的memberships，可以使用personId或personEmail(只能选其一)对此查询结果过滤,只能输入一个personId或personEmail，不可过滤多个
    //如果isFilterByPersonId为false,则过滤条件为邮箱
    void sendGetMemberships(QString roomId = "", QString filterString = "", bool isFilterByPersonId = true);

    //https://developer.webex.com/docs/api/v1/memberships/create-a-membership
    //Add someone to a room by Person ID or email address; optionally making them a moderator.
    //Default by Email, you can change it to Person ID by setting  "isByEmail" to false.
    void sendCreateAMembership(QString roomId, bool isModerator, QString byString, bool isByEmail =  true);

    //https://developer.webex.com/docs/api/v1/memberships/get-membership-details
    void sendGetMembership(QString membershipId);

    //https://developer.webex.com/docs/api/v1/memberships/update-a-membership
    void sendUpdateMembership(QString membershipId, bool isModerator);

    //https://developer.webex.com/docs/api/v1/memberships/delete-a-membership
    void sendDeleteMembership(QString membershipId);
//---------------Messages----------------------------------
    //https://developer.webex.com/docs/api/v1/messages/list-messages
    //Lists all messages in a room.
    //mentionedPeople: List messages with these people mentioned, by ID. Use me as a shorthand for the current API user.
    //before:List messages sent before a date and time. e.g. "2016-04-21T19:01:55.966Z"
    //beforeMessage:List messages sent before a message, by ID.
    //如果要获取group的messages,要把mentionedPeople置成“me”
    void sendListMessages(QString roomId,
                          QString mentionedPeople = "",
                          QString before = "",
                          QString beforeMessage = "",
                          int max = 50);

    //https://developer.webex.com/docs/api/v1/messages/list-direct-messages
    //Default by Email, you can change it to Person ID by setting  "isByEmail" to false.
    void sendListDirectMessages(QString byString, bool isByEmail = true);

    //https://developer.webex.com/docs/api/v1/messages/create-a-message
    //如果同时发送了markdown字段和text字段，text字段将被忽略，并将markdown解释后显示的字段作为text
    //现在只支持一次发送单个文件
    void sendCreateMessage(const BotMessage & message);

    //https://developer.webex.com/docs/api/v1/messages/get-message-details
    void sendGetMessageDetails(QString messageId);

    //https://developer.webex.com/docs/api/v1/messages/delete-a-message
    void sendDeleteMessage(QString messageId);

//---------------People----------------------------------
    //https://developer.webex.com/docs/api/v1/people/list-people
    //有三种方式，email或者displayName只能提供一个，id一次可以提供不超过85个
    //byWhat: 0--email,1-displayName,2-id
    void sendListPeople(QString byString, int byWhat, int max = 100);

    //https://developer.webex.com/docs/api/v1/people/get-person-details
    void sendGetPersonDetails(QString personId);

//---------------Rooms----------------------------------
    //https://developer.webex.com/docs/api/v1/rooms/list-rooms
    //type: e.g. group,direct
    //sortBy: e.g. id, lastactivity, created
    void sendListRooms(QString type = {}, QString sortBy = {}, int max = 100);

    //https://developer.webex.com/docs/api/v1/rooms/create-a-room
    void sendCreateRoom(QString title);

    //https://developer.webex.com/docs/api/v1/rooms/get-room-details
    void sendGetRoomDetails(QString roomId);

    //https://developer.webex.com/docs/api/v1/rooms/update-a-room
    void sendUpdateRoom(QString roomId, QString title);

    //https://developer.webex.com/docs/api/v1/rooms/delete-a-room
    void sendDeleteRoom(QString roomId);

//---------------Webhooks----------------------------------
    //https://developer.webex.com/docs/api/v1/webhooks/list-webhooks
    void sendListWebhooks(int max = 100);

    //https://developer.webex.com/docs/api/v1/webhooks/create-a-webhook
    void sendCreateWebhook(BotWebhook & webhook);

    //https://developer.webex.com/docs/api/v1/webhooks/get-webhook-details
    void sendGetWebhookDetails(QString webhookId);

    //https://developer.webex.com/docs/api/v1/webhooks/update-a-webhook
    void sendUpdateWebhook(QString webhookId, QString name, QString targetUrl, QString status = "active");

    //https://developer.webex.com/docs/api/v1/webhooks/delete-a-webhook
    void sendDeleteWebhook(QString webhookId);

private slots:
    void on_GetNgrokInfo(BotNetworkReplyHelper * nrh);
    void on_GetMemberships(BotNetworkReplyHelper * nrh);
    void on_CreateAMembership(BotNetworkReplyHelper * nrh);
    void on_GetMembership(BotNetworkReplyHelper * nrh);
    void on_UpdateMembership(BotNetworkReplyHelper * nrh);
    void on_DeleteMembership(BotNetworkReplyHelper * nrh);
    void on_ListMessages(BotNetworkReplyHelper * nrh);
    void on_ListDirectMessages(BotNetworkReplyHelper * nrh);
    void on_CreateMessage(BotNetworkReplyHelper * nrh);
    void on_GetMessageDetails(BotNetworkReplyHelper * nrh);
    void on_DeleteMessage(BotNetworkReplyHelper * nrh);
    void on_ListPeople(BotNetworkReplyHelper * nrh);
    void on_GetPersonDetails(BotNetworkReplyHelper * nrh);
    void on_ListRooms(BotNetworkReplyHelper * nrh);
    void on_CreateRoom(BotNetworkReplyHelper * nrh);
    void on_GetRoomDetails(BotNetworkReplyHelper * nrh);
    void on_UpdateRoom(BotNetworkReplyHelper * nrh);
    void on_DeleteRoom(BotNetworkReplyHelper * nrh);
    void on_ListWebhooks(BotNetworkReplyHelper * nrh);
    void on_CreateWebHook(BotNetworkReplyHelper * nrh);
    void on_GetWebhookDetails(BotNetworkReplyHelper * nrh);
    void on_UpdateWebhook(BotNetworkReplyHelper * nrh);
    void on_DeleteWebhook(BotNetworkReplyHelper * nrh);

private:
    explicit BotNetworkManager();

    void SendAndConnect(SendType type,
                        std::shared_ptr<QNetworkRequest> request,
                        void (BotNetworkManager::*fuc)(BotNetworkReplyHelper *),
                        const QByteArray & data
                        );
    void SendAndConnect(SendType type,
                        std::shared_ptr<QNetworkRequest> request,
                        void (BotNetworkManager::*fuc)(BotNetworkReplyHelper *),
                        QHttpMultiPart * data = nullptr
                        );

     std::shared_ptr<QJsonObject> ExtractContect(BotNetworkReplyHelper * nrh);

private:
    std::shared_ptr<QNetworkAccessManager> networkAccessManager;
};

#define NETMANAGER BotNetworkManager::Instance()

#endif // BOTNETWORKMANAGER_H
