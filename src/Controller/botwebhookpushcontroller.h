#ifndef BOTWEBHOOKPUSHCONTROLLER_H
#define BOTWEBHOOKPUSHCONTROLLER_H

#include <QObject>

#include <memory>
#include <bothttpclient.h>

class BotWebhookPush;
class BotPingController;

class BotWebhookPushController : public QObject
{
    Q_OBJECT
public:
    explicit BotWebhookPushController(QObject *parent = nullptr);

signals:
    void messageDetailsReady();

public slots:
    void on_webhookPushReady(std::shared_ptr<BotWebhookPush> object);
    void on_messageReady(std::shared_ptr<BotMessage> object);

private:
    bool CheckExist(std::shared_ptr<BotWebhookPush> object);
    void ProcessMessages(std::shared_ptr<BotWebhookPush> object);
    void ProcessMessagesCreated(std::shared_ptr<BotWebhookPush> object);

    bool CheckExistAndRemove(QString id, QString event);

private:
    std::vector<std::shared_ptr<BotWebhookPush>> webhookPushStore;
    std::vector<std::shared_ptr<BotPingController>> pingControllers;
};

#endif // BOTWEBHOOKPUSHCONTROLLER_H
