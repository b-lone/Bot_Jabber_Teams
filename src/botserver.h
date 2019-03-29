#ifndef BOTSERVER_H
#define BOTSERVER_H

#include <QObject>
#include <QList>
#include <QVector>

class QTcpServer;
class QTcpSocket;
class BotMessage;

class BotServer : public QObject
{
public:
    static BotServer * Instance();
    bool Listen();
    void Close();

private:
    BotServer();

public slots:
    void OnNewMessage(std::shared_ptr<BotMessage> message);

private slots:
    void on_newConnection();
    void on_readyRead();
    void on_disconnected();

private:
    std::shared_ptr<QTcpServer> server;
    QList<std::shared_ptr<QTcpSocket>> tcpClient;
    std::shared_ptr<QTcpSocket> currentClient;

    QVector<QString> messageIds;
};

#define BOTSERVER BotServer::Instance()

#endif // BOTSERVER_H
