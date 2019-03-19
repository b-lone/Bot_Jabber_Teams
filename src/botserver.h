#ifndef BOTSERVER_H
#define BOTSERVER_H

#include <QObject>
#include <QList>

class QTcpServer;
class QTcpSocket;

class BotServer : public QObject
{
public:
    static BotServer * Instance();
    bool Listen();
    void Close();

private:
    BotServer();

private slots:
    void on_newConnection();
    void on_readyRead();
    void on_disconnected();

private:
    std::shared_ptr<QTcpServer> server;
    QList<std::shared_ptr<QTcpSocket>> tcpClient;
    std::shared_ptr<QTcpSocket> currentClient;
};

#define SERVER BotServer::Instance()

#endif // BOTSERVER_H
