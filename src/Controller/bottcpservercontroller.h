#ifndef BOTSERVERCONTROLLER_H
#define BOTSERVERCONTROLLER_H

#include <QObject>
#include <memory>

class BotWebhookPush;

class BotTcpServerController : public QObject
{
    Q_OBJECT
public:
    explicit BotTcpServerController(QObject *parent = nullptr);

    void ProcessingNgrokData(std::shared_ptr<QByteArray> data);

signals:
    void webhookPushReady(std::shared_ptr<BotWebhookPush> object);

public slots:
};

#endif // BOTSERVERCONTROLLER_H
