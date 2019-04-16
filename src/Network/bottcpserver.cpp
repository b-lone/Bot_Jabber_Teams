#include "bottcpserver.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

#include "botconfig.h"
#include "botcommon.h"
#include "bothttpclient.h"
#include "botmessage.h"
#include "botstore.h"
#include "botconfig.h"
#include "bottcpservercontroller.h"

BotTcpSocketHelper::BotTcpSocketHelper(QTcpSocket *ts): QObject (ts)
{
    tcpSocket = std::shared_ptr<QTcpSocket>(ts);
    connect(ts, &QTcpSocket::readyRead, this, &BotTcpSocketHelper::on_readyRead);
    connect(ts, &QTcpSocket::disconnected, this, &BotTcpSocketHelper::on_disconnected);
}

void BotTcpSocketHelper::on_readyRead()
{
    emit readyRead(this);
}

void BotTcpSocketHelper::on_disconnected()
{
    emit disconnected(this);
}

//--------------------------------------------------------------------------------------------
BotTcpServer *BotTcpServer::Instance()
{
    static BotTcpServer * s = new BotTcpServer();
    return s;
}

bool BotTcpServer::Listen()
{
    bool result = true;

    {
        auto portNgrok = static_cast<unsigned short>(S_CONFIG->Value(BotConfig::ngrokPort).toInt());
        BOTLOG("ngrok port:" << portNgrok);
        if(! serverNgrok->listen(QHostAddress::Any, portNgrok)){
            result = false;
            BOTLOG("Ngrok server listen failed!");
        }
    }
    {
        auto portAutomation = static_cast<unsigned short>(S_CONFIG->Value(BotConfig::automationPort).toInt());
        BOTLOG("automation port:" << portAutomation);
        if(! serverAutomation->listen(QHostAddress::Any, portAutomation)){
            result = false;
            BOTLOG("Automation server listen failed!");
        }
    }

    return result;
}

void BotTcpServer::Close()
{
    BOTLOG("Close");
    {
        for(auto client: tcpClientNgrok)
        {
            client->disconnectFromHost();
            if(!client->waitForDisconnected(1000))
            {
                BOTLOG("Ngrok disconnect failed!");
            }
        }
        tcpClientNgrok.clear();
        serverNgrok->close();
    }
    {
        for(auto client: tcpClientAutomation)
        {
            client->disconnectFromHost();
            if(!client->waitForDisconnected(1000))
            {
                BOTLOG("Automation disconnect failed!");
            }
        }
        tcpClientAutomation.clear();
        serverAutomation->close();
    }
}

BotTcpServer::BotTcpServer()
{
    serverNgrok = std::make_shared<QTcpServer>();
    connect(serverNgrok.get(), &QTcpServer::newConnection, this, &BotTcpServer::on_newConnectionNgrok);

    serverAutomation = std::make_shared<QTcpServer>();
    connect(serverAutomation.get(), &QTcpServer::newConnection, this, &BotTcpServer::on_newConnectionAutomation);

    serverController = new BotTcpServerController(this);
}

void BotTcpServer::on_newConnectionNgrok()
{
    BOTLOG("New Connection!");
//    std::shared_ptr<QTcpSocket> currentClient(serverNgrok->nextPendingConnection());
//    tcpClientNgrok.append(currentClient);

//    BotTcpSocketHelper * tsh = new BotTcpSocketHelper(currentClient.get());;
//    connect(tsh, &BotTcpSocketHelper::readyRead, this, &BotServer::on_readyReadNgrok);
//    connect(tsh, &BotTcpSocketHelper::disconnected, this, &BotServer::on_disconnectedNgrok);
    auto currentClient = serverNgrok->nextPendingConnection();
    tcpClientNgrok.append(currentClient);

    connect(currentClient, &QTcpSocket::readyRead, this, &BotTcpServer::on_readyReadNgrok);
    connect(currentClient, &QTcpSocket::disconnected, this, &BotTcpServer::on_disconnectedNgrok);
}

void BotTcpServer::on_readyReadNgrok()
{
    BOTLOG("New Webhook Push.");

    for (int i = 0; i < tcpClientNgrok.count(); ++i) {
        std::shared_ptr<QByteArray> data(new QByteArray(tcpClientNgrok[i]->readAll()));
        serverController->ProcessingNgrokData(data);
    }
}
void BotTcpServer::on_disconnectedNgrok()
{
    BOTLOG("Disconnected.");
//    tcpClientNgrok.removeOne(nrh->GetTcpSocket());
    for(int i=0; i<tcpClientNgrok.length(); i++)
    {
        if(tcpClientNgrok[i]->state() == QAbstractSocket::UnconnectedState)
        {
            // 删除存储在tcpClient列表中的客户端信息
            tcpClientNgrok.removeAt(i);
        }
    }
}

void BotTcpServer::on_newConnectionAutomation()
{
    BOTLOG("New Connection!");

    auto currentClient = serverAutomation->nextPendingConnection();
    tcpClientAutomation.append(currentClient);

    connect(currentClient, &QTcpSocket::readyRead, this, &BotTcpServer::on_readyReadAutomation);
    connect(currentClient, &QTcpSocket::disconnected, this, &BotTcpServer::on_disconnectedAutomation);
}

void BotTcpServer::on_readyReadAutomation()
{
    BOTLOG("New Automation Message.");

    for (int i = 0; i < tcpClientAutomation.count(); ++i) {
        QByteArray buffer = tcpClientAutomation[i]->readAll();
        if(buffer.isEmpty())
            continue;
    }
}

void BotTcpServer::on_disconnectedAutomation()
{
    BOTLOG("Disconnected.");
    for(int i=0; i<tcpClientAutomation.length(); i++)
    {
        if(tcpClientAutomation[i]->state() == QAbstractSocket::UnconnectedState)
        {
            // 删除存储在tcpClient列表中的客户端信息
            tcpClientAutomation.removeAt(i);
        }
    }
}
