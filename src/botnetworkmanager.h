#ifndef BOTNETWORKMANAGER_H
#define BOTNETWORKMANAGER_H

#include <QObject>
#include <QJsonObject>
class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;

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

class BotNetworkManager : public QObject
{
    Q_OBJECT
public:
    static BotNetworkManager * Instance();
    void sendGetRooms();
    //https://developer.webex.com/docs/api/v1/memberships/list-memberships
    void sendGetMemberships(QString queryParameters = nullptr);
    //https://developer.webex.com/docs/api/v1/memberships/create-a-membership
    void sendCreateAMembership(QString roomId, QString personEmail, bool isModerator);
    //https://developer.webex.com/docs/api/v1/memberships/get-membership-details
    void sendGetMembership(QString membershipId);
    //https://developer.webex.com/docs/api/v1/memberships/update-a-membership
    void sendUpdateMembership(QString membershipId, bool isModerator);
    //https://developer.webex.com/docs/api/v1/memberships/delete-a-membership
    void sendDeleteMembership(QString membershipId);
    //https://developer.webex.com/docs/api/v1/messages/list-messages
    void sendListMessages(QString roomId);
    //https://developer.webex.com/docs/api/v1/messages/list-direct-messages
    void sendListDirectMessages(QString personEmail);
    //https://developer.webex.com/docs/api/v1/messages/create-a-message
    void sendCreateMessage(QString roomId,QString text, QString files = "");
    //https://developer.webex.com/docs/api/v1/messages/get-message-details
    void sendGetMessageDetails(QString messageId);
    //https://developer.webex.com/docs/api/v1/messages/delete-a-message
    void sendDeleteMessage(QString messageId);


private slots:
    void on_GetRooms(BotNetworkReplyHelper * nrh);
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

private:
    explicit BotNetworkManager();
    void SetHeaderAuthorization( std::shared_ptr<QNetworkRequest> request);
    std::shared_ptr<QNetworkRequest> NewRequest(const QUrl & url);
    QJsonObject ExtractContect(BotNetworkReplyHelper * nrh);

private:
    std::shared_ptr<QNetworkAccessManager> networkAccessManager;
};

#define NETMANAGER BotNetworkManager::Instance()

#endif // BOTNETWORKMANAGER_H
