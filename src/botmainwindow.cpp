#include "botmainwindow.h"
#include "ui_botmainwindow.h"

#include "botnetworkmanager.h"
#include "botconfig.h"
#include "botserver.h"
#include <QDebug>

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
    NETMANAGER->sendGetRooms();
}

void BotMainWindow::on_btnMemberships_clicked()
{
    NETMANAGER->sendGetMemberships();
}

void BotMainWindow::on_btnAddMem_clicked()
{
    NETMANAGER->sendCreateAMembership(
                "Y2lzY29zcGFyazovL3VzL1JPT00vN2NlNzBkODAtNGExMC0xMWU5LTg1OWEtOWQwMGI2ODRlNGI0",
                "guyqu@cisco.com",
                false);
}

void BotMainWindow::on_btnMembership_clicked()
{
    NETMANAGER->sendGetMembership("Y2lzY29zcGFyazovL3VzL01FTUJFUlNISVAvN2U2MDlmNGMtYTQyYi00Mjc4LWE1NmYtMzM1ZjYxOTA1NzAwOjNkMWJlMTA1LWVkYWItM2RjYi1hYTM1LTE3ZjU1OGQ4NmEyNQ");
}

void BotMainWindow::on_btnUpdateMem_clicked()
{
    NETMANAGER->sendUpdateMembership("Y2lzY29zcGFyazovL3VzL01FTUJFUlNISVAvN2YzZGZkMWUtNzg1Ny00YTg4LTliODgtYzdlZjQ4MTg3YmQ4OjdjZTcwZDgwLTRhMTAtMTFlOS04NTlhLTlkMDBiNjg0ZTRiNA", true);
}

void BotMainWindow::on_btnDeleteMem_clicked()
{
    NETMANAGER->sendDeleteMembership("Y2lzY29zcGFyazovL3VzL01FTUJFUlNISVAvZDVmZDVhMTUtZjcyYy00NTM4LTg2ZDEtYmJhYjhhMjFlMTk1OjdjZTcwZDgwLTRhMTAtMTFlOS04NTlhLTlkMDBiNjg0ZTRiNA");
}

void BotMainWindow::on_btnListMsg_clicked()
{
    NETMANAGER->sendListMessages("Y2lzY29zcGFyazovL3VzL1JPT00vYTE4N2ZiOTItZDYxOC0zMzlmLWJkZWQtMDdhOWZmMWI0YzM1");
}

void BotMainWindow::on_btnListDMsg_clicked()
{
    NETMANAGER->sendListDirectMessages("kuyou@cisco.com");
}

void BotMainWindow::on_btnCreateMsg_clicked()
{
    NETMANAGER->sendCreateMessage("Y2lzY29zcGFyazovL3VzL1JPT00vN2NlNzBkODAtNGExMC0xMWU5LTg1OWEtOWQwMGI2ODRlNGI0",
                                  "Hello World"/*,
                                  "fengqiuhuang.png"*/);
}

void BotMainWindow::on_btnGetMsg_clicked()
{
    NETMANAGER->sendGetMessageDetails("Y2lzY29zcGFyazovL3VzL01FU1NBR0UvNDAyOTBkYTAtNDNkOC0xMWU5LTlmY2QtYWJmM2QwMDA3NDIx");
}

void BotMainWindow::on_btnDeleteMsg_clicked()
{
    NETMANAGER->sendDeleteMessage("Y2lzY29zcGFyazovL3VzL01FU1NBR0UvMTA3NDM0ZDAtNGJkMS0xMWU5LTliYjMtODE0YzI2MDRhOWE3");
}
