#ifndef BOTWEBHOOKCONTROLLER_H
#define BOTWEBHOOKCONTROLLER_H

#include <QObject>

#include <memory>
#include <botnetworkmanager.h>

class BotWebhook;

class BotWebhookController : public QObject
{
    Q_OBJECT
public:
    explicit BotWebhookController(QObject *parent = nullptr);

private:
    bool ParseData(std::shared_ptr<QByteArray> data, QString &URL);

signals:

public slots:
    void on_ngrokReady(std::shared_ptr<QByteArray> data);
    void on_webhookReady(std::shared_ptr<BotWebhook> object);
    void on_emptyList(RequestType rt);

private:
    QString ngrokUrl;
};

#endif // BOTWEBHOOKCONTROLLER_H
