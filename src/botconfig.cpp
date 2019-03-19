#include "botconfig.h"
#include <QSettings>

const QString BotConfig::AccessToken = "bot/Access Token";

BotConfig *BotConfig::Instance()
{
    static BotConfig * botConfig = new BotConfig();
    return botConfig;
}

QVariant BotConfig::Value(const QString &key)
{
    return settings->value(key);
}

void BotConfig::SetValue(const QString &key, const QVariant &value)
{
    settings->setValue(key, value);
}

BotConfig::BotConfig()
{
    this->settings = std::make_shared<QSettings>("./config/config.ini", QSettings::IniFormat);
}
