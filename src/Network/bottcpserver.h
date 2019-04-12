#ifndef BOTTCPSERVER_H
#define BOTTCPSERVER_H

#include <QObject>
#include <memory>
#include <QList>
#include <QVector>

class QTcpServer;
class QTcpSocket;
class BotMessage;
class BotTcpServerController;

class BotTcpSocketHelper : public QObject
{
    Q_OBJECT
public:
    BotTcpSocketHelper(QTcpSocket * ts);
    std::shared_ptr<QTcpSocket> GetTcpSocket(){ return tcpSocket.lock(); }

signals:
    void readyRead(BotTcpSocketHelper *);
    void disconnected(BotTcpSocketHelper *);

private slots:
    void on_readyRead();
    void on_disconnected();

private:
    std::weak_ptr<QTcpSocket> tcpSocket;
};


class BotTcpServer : public QObject
{
    Q_OBJECT
public:
    static BotTcpServer * Instance();
    bool Listen();
    void Close();

    BotTcpServerController * getTcpServerController() { return serverController; }

private:
    BotTcpServer();

private slots:
    void on_newConnectionNgrok();
    void on_readyReadNgrok();
    void on_disconnectedNgrok();

    void on_newConnectionAutomation();
    void on_readyReadAutomation();
    void on_disconnectedAutomation();
private:
    std::shared_ptr<QTcpServer> serverNgrok;
    QList<QTcpSocket*> tcpClientNgrok;

    QVector<QString> messageIds;

    std::shared_ptr<QTcpServer> serverAutomation;
    QList<QTcpSocket*> tcpClientAutomation;

    BotTcpServerController *serverController;
};

#define S_TCPSERVER BotTcpServer::Instance()

#endif // BOTTCPSERVER_H
