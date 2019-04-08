#ifndef BOTNETWORKCONTROLLER_H
#define BOTNETWORKCONTROLLER_H

#include <QObject>

#include <botnetworkmanager.h>

class BotRoom;
class BotPeople;
class BotMessage;
class BotWebhook;
class BotMembership;

class BotNetworkController : public QObject
{
    Q_OBJECT
public:
    explicit BotNetworkController(QObject *parent = nullptr);

signals:
    void roomReady(std::shared_ptr<BotRoom>);
    void peopleReady(std::shared_ptr<BotPeople>);
    void messageReady(std::shared_ptr<BotMessage>);
    void webhookReady(std::shared_ptr<BotWebhook>);
    void membershipReady(std::shared_ptr<BotMembership>);
    void emptyList(RequestType);

public slots:
    void on_dataReady(std::shared_ptr<QByteArray> data, RequestType requestType);
    
private:
    void Distribute(QJsonObject * jsonObject, RequestType requestType);
    void Rooms(QJsonObject * jsonObject);
    void People(QJsonObject * jsonObject);
    void Messages(QJsonObject * jsonObject);
    void Webhooks(QJsonObject * jsonObject);
    void Memberships(QJsonObject * jsonObject);
};

#endif // BOTNETWORKCONTROLLER_H
