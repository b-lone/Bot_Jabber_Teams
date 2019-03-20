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
    void sendGetMemberships(QString * queryParameters = nullptr);
    //https://developer.webex.com/docs/api/v1/memberships/create-a-membership
    void sendCreateAMembership(QString roomId, QString personEmail, bool isModerator);

private slots:
    void on_GetRooms(BotNetworkReplyHelper * nrh);
    void on_GetMemberships(BotNetworkReplyHelper * nrh);
    void on_CreateAMembership(BotNetworkReplyHelper * nrh);

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
