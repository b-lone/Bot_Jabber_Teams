#ifndef BOTSERVERCONTROLLER_H
#define BOTSERVERCONTROLLER_H

#include <QObject>
#include <memory>

class BotWebhookPush;
class BotCommand;

class BotTcpServerController : public QObject
{
    Q_OBJECT
public:
    explicit BotTcpServerController(QObject *parent = nullptr);

    void ProcessingNgrokData(std::shared_ptr<QByteArray> data);
    void ProcessingAutomationData(std::shared_ptr<QByteArray> data);

signals:
    void webhookPushReady(std::shared_ptr<BotWebhookPush>);
    void commandReady(std::shared_ptr<BotCommand>);
};

#endif // BOTSERVERCONTROLLER_H
