#include "botmainwindow.h"
#include "ui_botmainwindow.h"

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
