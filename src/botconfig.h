#ifndef BOTCONFIG_H
#define BOTCONFIG_H

#include <QString>
#include <QVariant>

class QSettings;
class BotConfig
{
public:
    static BotConfig * Instance();

    static const QString BotAccessToken;
    static const QString BotId;
    static const QString BotDisplayName;
    static const QString ngrokPort;
    static const QString ngrokSubdomain;
    static const QString automationPort;

    QVariant Value(const QString & key);
    void SetValue(const QString & key, const QVariant & value);
private:
    BotConfig();
    std::shared_ptr<QSettings> settings;
};

#define BOTCONFIG BotConfig::Instance()

#endif // BOTCONFIG_H
