#ifndef BOTCONFIG_H
#define BOTCONFIG_H

#include <QString>
#include <QVariant>

class QSettings;
class BotConfig
{
public:
    static BotConfig * Instance();

    static const QString AccessToken;
    static const QString ListenPort;
    static const QString BotId;
    static const QString BotDisplayName;

    QVariant Value(const QString & key);
    void SetValue(const QString & key, const QVariant & value);
private:
    BotConfig();
    std::shared_ptr<QSettings> settings;
};

#define BOTCONFIG BotConfig::Instance()

#endif // BOTCONFIG_H
