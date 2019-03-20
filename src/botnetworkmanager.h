#ifndef BOTNETWORKMANAGER_H
#define BOTNETWORKMANAGER_H

#include <QObject>
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

private slots:
    void on_GetRooms(BotNetworkReplyHelper * nrh);

private:
    explicit BotNetworkManager();
    void SetHeaderAuthorization(QNetworkRequest & request);

private:
    std::shared_ptr<QNetworkAccessManager> networkAccessManager;
};

#define NETMANAGER BotNetworkManager::Instance()

#endif // BOTNETWORKMANAGER_H
