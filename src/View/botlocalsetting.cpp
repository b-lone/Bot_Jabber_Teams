#include "botlocalsetting.h"
#include "ui_botlocalsetting.h"

#include "botconfig.h"

const QString prefix = "Bearer ";

BotLocalSetting::BotLocalSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BotLocalSetting)
{
    ui->setupUi(this);
    Init();
}

BotLocalSetting::~BotLocalSetting()
{
    delete ui;
}

void BotLocalSetting::on_btnReset_clicked()
{
    Init();
}

void BotLocalSetting::on_btnOK_clicked()
{

}

void BotLocalSetting::Init()
{
    ui->leId->setText(S_CONFIG->Value(BotConfig::BotId).toString());
    auto accessToken = S_CONFIG->Value(BotConfig::BotAccessToken).toString();
    auto subString = accessToken.mid(prefix.size());
    ui->leAccessToken->setText(subString);
    ui->lePortNgrok->setText(S_CONFIG->Value(BotConfig::ngrokPort).toString());
    ui->lePortAuto->setText(S_CONFIG->Value(BotConfig::automationPort).toString());
}
