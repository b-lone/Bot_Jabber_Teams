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
