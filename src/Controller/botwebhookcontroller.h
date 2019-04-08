#ifndef BOTWEBHOOKCONTROLLER_H
#define BOTWEBHOOKCONTROLLER_H

#include <QObject>

#include <QString>

class BotWebhookController : public QObject
{
    Q_OBJECT
public:
    static BotWebhookController * Instance();

private:
    explicit BotWebhookController(QObject *parent = nullptr);

signals:

public slots:
    void on_getNgrokURL(QString url);
private:
    QString ngrokUrl;
};

#define BOTWEBHOOKCONTROLLER BotConfig::Instance()

#endif // BOTWEBHOOKCONTROLLER_H
