#ifndef BOTSERVER_H
#define BOTSERVER_H

#include <QObject>
#include <QList>
#include <QVector>

class QTcpServer;
class QTcpSocket;
class BotMessage;

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
    void on_newConnectionNgrok();
    void on_readyReadNgrok(BotTcpSocketHelper * nrh);
    void on_disconnectedNgrok(BotTcpSocketHelper * nrh);

    void on_newConnectionAutomation();
    void on_readyReadAutomation(BotTcpSocketHelper * nrh);
    void on_disconnectedAutomation(BotTcpSocketHelper * nrh);
private:
    std::shared_ptr<QTcpServer> serverNgrok;
    QList<std::shared_ptr<QTcpSocket>> tcpClientNgrok;

    QVector<QString> messageIds;

    std::shared_ptr<QTcpServer> serverAutomation;
    QList<std::shared_ptr<QTcpSocket>> tcpClientAutomation;
};

#define BOTSERVER BotServer::Instance()

#endif // BOTSERVER_H
