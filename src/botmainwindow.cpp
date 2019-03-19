#include "botmainwindow.h"
#include "ui_botmainwindow.h"

#include "botnetworkmanager.h"
#include "botconfig.h"

BotMainWindow::BotMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BotMainWindow)
{
    ui->setupUi(this);
}

BotMainWindow::~BotMainWindow()
{
    delete ui;
}

void BotMainWindow::on_btnSend_clicked()
{
    NETMANAGER->sendGetRooms();
}
