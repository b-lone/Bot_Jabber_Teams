#ifndef BOTSERVERCONTROLLER_H
#define BOTSERVERCONTROLLER_H

#include <QObject>
#include <memory>

class BotWebhookPush;
class BotObject;
typedef struct {
    std::shared_ptr<BotObject> object;
}Command;

class BotTcpServerController : public QObject
{
    Q_OBJECT
public:
    explicit BotTcpServerController(QObject *parent = nullptr);

    void ProcessingNgrokData(std::shared_ptr<QByteArray> data);
    void ProcessingAutomationData(std::shared_ptr<QByteArray> data);

signals:
    void webhookPushReady(std::shared_ptr<BotWebhookPush> object);
    void automationCommandReady();
};

#endif // BOTSERVERCONTROLLER_H
