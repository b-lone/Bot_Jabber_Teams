#ifndef BOTNETWORKMANAGER_H
#define BOTNETWORKMANAGER_H

#include <QObject>
class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;

class BotNetworkManager : public QObject
{
    Q_OBJECT
public:
    static BotNetworkManager * Instance();
    void sendGetRooms();

private slots:
    void on_GetRooms();

private:
    explicit BotNetworkManager();
    void SetHeaderAuthorization(QNetworkRequest & request);

private:
    std::shared_ptr<QNetworkAccessManager> networkAccessManager;
    QNetworkReply * replyGetRooms = nullptr;

};

#define NETMANAGER BotNetworkManager::Instance()

#endif // BOTNETWORKMANAGER_H
