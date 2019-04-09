#ifndef BOTSTORE_H
#define BOTSTORE_H

#include <QObject>
#include <vector>
#include <memory>

class BotRoom;
class BotMessage;
class BotMembership;
class BotPeople;
class BotWebhook;

class BotStore : public QObject
{
    Q_OBJECT
public:
    static BotStore * Instance();

    const std::vector<std::shared_ptr<BotWebhook>> & getWebhooks() { return webhookStore; }
    std::shared_ptr<BotWebhook> getWebhookById(QString id);

    void PushMessage(std::shared_ptr<BotMessage> object);
    void PushRoom(std::shared_ptr<BotRoom> object);
    void PushMembership(std::shared_ptr<BotMembership> object);
    void PushPeople(std::shared_ptr<BotPeople> object);
    void PushWebhook(std::shared_ptr<BotWebhook> object);

private:
    explicit BotStore(QObject *parent = nullptr);

signals:
   void MessageReady(std::shared_ptr<BotMessage>);
   void RoomReady(std::shared_ptr<BotRoom>);
   void MembershipReady(std::shared_ptr<BotMembership>);
   void PeopleReady(std::shared_ptr<BotPeople>);
   void WebhookReady();

public slots:
   void on_webhookListReady(std::vector<std::shared_ptr<BotWebhook>> objects);

private:
    std::vector<std::shared_ptr<BotRoom>> roomStore;
    std::vector<std::shared_ptr<BotMessage>> messagesStore;
    std::vector<std::shared_ptr<BotMembership>> membershipStore;
    std::vector<std::shared_ptr<BotPeople>> peopleStore;
    std::vector<std::shared_ptr<BotWebhook>> webhookStore;
};

#define BOTSTORE BotStore::Instance()

#endif // BOTSTORE_H
