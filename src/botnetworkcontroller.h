#ifndef BOTNETWORKCONTROLLER_H
#define BOTNETWORKCONTROLLER_H

#include <QObject>

#include <botnetworkmanager.h>

class BotNetworkController : public QObject
{
    Q_OBJECT
public:
    explicit BotNetworkController(QObject *parent = nullptr);

signals:

public slots:
    void on_dataReady(const QByteArray & data, RequestType requestType);
    
private:
    void Distribute(QJsonObject * jsonObject, RequestType requestType);
    void Rooms(QJsonObject * jsonObject);
    void People(QJsonObject * jsonObject);
    void Messages(QJsonObject * jsonObject);
    void Webhooks(QJsonObject * jsonObject);
    void Memberships(QJsonObject * jsonObject);
};

#endif // BOTNETWORKCONTROLLER_H
