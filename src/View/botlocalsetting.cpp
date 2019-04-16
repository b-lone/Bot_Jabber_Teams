#include "botlocalsetting.h"
#include "ui_botlocalsetting.h"

#include "botconfig.h"
#include "botcommon.h"

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
    auto botId = ui->leId->text();
    auto botAccessToken = prefix + ui->leAccessToken->text();
    auto displayName = ui->leDisplayName->text();
    auto portNgrok = ui->lePortNgrok->text();
    auto portAuto = ui->lePortAuto->text();

    auto y_botId = S_CONFIG->Value(BotConfig::BotId);
    auto y_botAccessToken = S_CONFIG->Value(BotConfig::BotAccessToken);
    auto y_displayName = S_CONFIG->Value(BotConfig::BotDisplayName);
    auto y_ngrokPort = S_CONFIG->Value(BotConfig::ngrokPort);
    auto y_autoPort = S_CONFIG->Value(BotConfig::automationPort);

    S_CONFIG->SetValue(BotConfig::BotId, botId);
    S_CONFIG->SetValue(BotConfig::BotAccessToken, botAccessToken);
    S_CONFIG->SetValue(BotConfig::BotDisplayName, displayName);
    S_CONFIG->SetValue(BotConfig::ngrokPort, portNgrok);
    S_CONFIG->SetValue(BotConfig::automationPort, portAuto);

    BOTLOG("BotId:" << botId);
    BOTLOG("BotAccessToken:" << botAccessToken);
    BOTLOG("BotDisplayName:" << displayName);
    BOTLOG("ngrokPort:" << portNgrok);
    BOTLOG("automationPort:" << portAuto);

    if(botId != y_botId
            || botAccessToken != y_botAccessToken
            || displayName != y_displayName
            || portNgrok != y_ngrokPort
            || portAuto != y_autoPort){
        emit needReboot();
    }
}

void BotLocalSetting::Init()
{
    BOTLOG("Init");
    ui->leId->setText(S_CONFIG->Value(BotConfig::BotId).toString());
    auto accessToken = S_CONFIG->Value(BotConfig::BotAccessToken).toString();
    auto subString = accessToken.mid(prefix.size());
    ui->leAccessToken->setText(subString);
    ui->leDisplayName->setText(S_CONFIG->Value(BotConfig::BotDisplayName).toString());
    ui->lePortNgrok->setText(S_CONFIG->Value(BotConfig::ngrokPort).toString());
    ui->lePortAuto->setText(S_CONFIG->Value(BotConfig::automationPort).toString());
}
