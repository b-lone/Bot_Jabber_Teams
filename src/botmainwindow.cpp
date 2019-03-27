#include "botmainwindow.h"
#include "ui_botmainwindow.h"

#include "botnetworkmanager.h"
#include "botconfig.h"
#include "botserver.h"
#include "botwebhook.h"
#include "botmessage.h"
#include "botcommon.h"

BotMainWindow::BotMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BotMainWindow)
{
    ui->setupUi(this);
    qDebug() << SERVER->Listen();
}

BotMainWindow::~BotMainWindow()
{
    delete ui;
}

void BotMainWindow::on_btnRooms_clicked()
{
    NETMANAGER->sendListRooms();
}

void BotMainWindow::on_btnMemberships_clicked()
{
    NETMANAGER->sendGetMemberships("Y2lzY29zcGFyazovL3VzL1JPT00vN2NlNzBkODAtNGExMC0xMWU5LTg1OWEtOWQwMGI2ODRlNGI0",
                                   "kuyou@cisco.com",
                                   false);
}

void BotMainWindow::on_btnAddMem_clicked()
{
    NETMANAGER->sendCreateAMembership(
                "Y2lzY29zcGFyazovL3VzL1JPT00vN2NlNzBkODAtNGExMC0xMWU5LTg1OWEtOWQwMGI2ODRlNGI0",
                true,
                "Y2lzY29zcGFyazovL3VzL1BFT1BMRS9hMTJhYjcyMS1jZWFlLTQ1MGYtOTA3YS1mYjY5NGQ1YTg3OTk",
                false
                );
}

void BotMainWindow::on_btnMembership_clicked()
{
    NETMANAGER->sendGetMembership("Y2lzY29zcGFyazovL3VzL01FTUJFUlNISVAvN2U2MDlmNGMtYTQyYi00Mjc4LWE1NmYtMzM1ZjYxOTA1NzAwOjNkMWJlMTA1LWVkYWItM2RjYi1hYTM1LTE3ZjU1OGQ4NmEyNQ");
}

void BotMainWindow::on_btnUpdateMem_clicked()
{
    NETMANAGER->sendUpdateMembership("Y2lzY29zcGFyazovL3VzL01FTUJFUlNISVAvMjY4NDVmNGUtMGExZS00ODQ5LTkyOGQtYjUyMmI1NjdkNWI5OjdjZTcwZDgwLTRhMTAtMTFlOS04NTlhLTlkMDBiNjg0ZTRiNA",
                                     false);
}

void BotMainWindow::on_btnDeleteMem_clicked()
{
    NETMANAGER->sendDeleteMembership("Y2lzY29zcGFyazovL3VzL01FTUJFUlNISVAvYTEyYWI3MjEtY2VhZS00NTBmLTkwN2EtZmI2OTRkNWE4Nzk5OjdjZTcwZDgwLTRhMTAtMTFlOS04NTlhLTlkMDBiNjg0ZTRiNA");
}

void BotMainWindow::on_btnListMsg_clicked()
{
    NETMANAGER->sendListMessages("Y2lzY29zcGFyazovL3VzL1JPT00vN2NlNzBkODAtNGExMC0xMWU5LTg1OWEtOWQwMGI2ODRlNGI0", "me");
}

void BotMainWindow::on_btnListDMsg_clicked()
{
    NETMANAGER->sendListDirectMessages("Y2lzY29zcGFyazovL3VzL1BFT1BMRS83ZjNkZmQxZS03ODU3LTRhODgtOWI4OC1jN2VmNDgxODdiZDg",
                                       false);
}

void BotMainWindow::on_btnCreateMsg_clicked()
{
    BotMessage message;
    message.roomId = "Y2lzY29zcGFyazovL3VzL1JPT00vN2NlNzBkODAtNGExMC0xMWU5LTg1OWEtOWQwMGI2ODRlNGI0";
//    message.toPersonId = "Y2lzY29zcGFyazovL3VzL1BFT1BMRS83ZjNkZmQxZS03ODU3LTRhODgtOWI4OC1jN2VmNDgxODdiZDg";
//    message.toPersonEmail = "kuyou@cisco.com";
    message.text = "Hello World";
    message.markdown = "**Title**";
//    message.files.append("CMakeFiles/feature_tests.bin");
    message.files.append("fengqiuhuang.png");
    NETMANAGER->sendCreateMessage(message);
}

void BotMainWindow::on_btnGetMsg_clicked()
{
    NETMANAGER->sendGetMessageDetails("Y2lzY29zcGFyazovL3VzL01FU1NBR0UvNDAyOTBkYTAtNDNkOC0xMWU5LTlmY2QtYWJmM2QwMDA3NDIx");
}

void BotMainWindow::on_btnDeleteMsg_clicked()
{
    NETMANAGER->sendDeleteMessage("Y2lzY29zcGFyazovL3VzL01FU1NBR0UvMTA3NDM0ZDAtNGJkMS0xMWU5LTliYjMtODE0YzI2MDRhOWE3");
}

void BotMainWindow::on_btnListPeople_clicked()
{
//    NETMANAGER->sendListPeople("kuyou@cisco.com");
//    NETMANAGER->sendListPeople("KunYou123@webex.bot", 0);
//    NETMANAGER->sendListPeople("Kun You", 1);
    NETMANAGER->sendListPeople("Y2lzY29zcGFyazovL3VzL1BFT1BMRS83ZjNkZmQxZS03ODU3LTRhODgtOWI4OC1jN2VmNDgxODdiZDg,Y2lzY29zcGFyazovL3VzL1BFT1BMRS9kNWZkNWExNS1mNzJjLTQ1MzgtODZkMS1iYmFiOGEyMWUxOTU,",
                               2);
}

void BotMainWindow::on_btnGetPersonDetails_clicked()
{
    NETMANAGER->sendGetPersonDetails("Y2lzY29zcGFyazovL3VzL1BFT1BMRS8zMDhmNmI4My1hOGFjLTQyNzItYjg5NS03NGIyYzY5NDQ1MTc");
    NETMANAGER->sendGetPersonDetails("me");
}

void BotMainWindow::on_btnCreateRoom_clicked()
{
    NETMANAGER->sendCreateRoom("test");//Y2lzY29zcGFyazovL3VzL1JPT00vOTRmNmZkMTAtNGM3NS0xMWU5LWI2NGUtNzFhNmQ5ZTMzODAz
}

void BotMainWindow::on_btnGetRoomDetails_clicked()
{
    NETMANAGER->sendGetRoomDetails("Y2lzY29zcGFyazovL3VzL1JPT00vN2NlNzBkODAtNGExMC0xMWU5LTg1OWEtOWQwMGI2ODRlNGI0");
}

void BotMainWindow::on_btnUpdateRoom_clicked()
{
    NETMANAGER->sendUpdateRoom("Y2lzY29zcGFyazovL3VzL1JPT00vN2NlNzBkODAtNGExMC0xMWU5LTg1OWEtOWQwMGI2ODRlNGI0", "Hello World");
}

void BotMainWindow::on_btnDeleteRoom_clicked()
{
    NETMANAGER->sendDeleteRoom("Y2lzY29zcGFyazovL3VzL1JPT00vOTRmNmZkMTAtNGM3NS0xMWU5LWI2NGUtNzFhNmQ5ZTMzODAz");
}

void BotMainWindow::on_btnListWebhooks_clicked()
{
    NETMANAGER->sendListWebhooks();
}

void BotMainWindow::on_btnCreateWebhook_clicked()
{
    BotWebhook webhook;
    webhook.name = "qt_create";
    webhook.targetUrl = "https://6eca506c.ngrok.io";
    webhook.resource = "messages";
    webhook.event = "created";

    NETMANAGER->sendCreateWebhook(webhook);
}

void BotMainWindow::on_btnWebhookDetails_clicked()
{
    NETMANAGER->sendGetWebhookDetails("Y2lzY29zcGFyazovL3VzL1dFQkhPT0svZTUwZThkMTctYzc3Ny00Mjc5LTgwNTctODkwMTU4MzIwYmU0");
}

void BotMainWindow::on_btnUpdateWebhook_clicked()
{
    NETMANAGER->sendUpdateWebhook("Y2lzY29zcGFyazovL3VzL1dFQkhPT0svZTUwZThkMTctYzc3Ny00Mjc5LTgwNTctODkwMTU4MzIwYmU0", "test", "https://6eca506c.ngrok.io");
}

void BotMainWindow::on_btnDeleteWebhook_clicked()
{
    NETMANAGER->sendDeleteWebhook("Y2lzY29zcGFyazovL3VzL1dFQkhPT0svZWMxZTFhYTQtZjA3NS00MjE3LTkzODUtZDA4M2ZjY2VhNTBi");
}
