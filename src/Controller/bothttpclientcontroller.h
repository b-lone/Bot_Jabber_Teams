#ifndef BOTNETWORKCONTROLLER_H
#define BOTNETWORKCONTROLLER_H

#include <QObject>

#include <vector>
#include <bothttpclient.h>

class BotRoom;
class BotPeople;
class BotMessage;
class BotWebhook;
class BotMembership;

class BotHttpClientController : public QObject
{
    Q_OBJECT
public:
    explicit BotHttpClientController(QObject *parent = nullptr);

    void ProcessingTSData(std::shared_ptr<QByteArray> data, RequestType requestType);
    void ProcessingNgrokData(std::shared_ptr<QByteArray> data);

signals:
    void roomReady(std::shared_ptr<BotRoom>);
    void peopleReady(std::shared_ptr<BotPeople>);
    void messageReady(std::shared_ptr<BotMessage>);
    void webhookReady(std::shared_ptr<BotWebhook>);
    void membershipReady(std::shared_ptr<BotMembership>);

    void roomListReady(std::vector<std::shared_ptr<BotRoom>>);
    void peopleListReady(std::vector<std::shared_ptr<BotPeople>>);
    void messageListReady(std::vector<std::shared_ptr<BotMessage>>);
    void webhookListReady(std::vector<std::shared_ptr<BotWebhook>>);
    void membershipListReady(std::vector<std::shared_ptr<BotMembership>>);

    void ngrokUrlReady(QString);
    
private:
    void RoomRoot(QJsonObject * jsonObject);
    void PeopleRoot(QJsonObject * jsonObject);
    void MessageRoot(QJsonObject * jsonObject);
    void WebhookRoot(QJsonObject * jsonObject);
    void MembershipRoot(QJsonObject * jsonObject);

    template<class T>
    std::shared_ptr<T> GenerateObject(QJsonObject * jsonObject);

    template<class T>
    std::vector<std::shared_ptr<T>> GenerateObjects(QJsonObject * jsonObject);

    void Rooms(QJsonObject * jsonObject);
    void People(QJsonObject * jsonObject);
    void Messages(QJsonObject * jsonObject);
    void Webhooks(QJsonObject * jsonObject);
    void Memberships(QJsonObject * jsonObject);

    void Room(QJsonObject * jsonObject);
    void Person(QJsonObject * jsonObject);
    void Message(QJsonObject * jsonObject);
    void Webhook(QJsonObject * jsonObject);
    void Membership(QJsonObject * jsonObject);
};

#endif // BOTNETWORKCONTROLLER_H
