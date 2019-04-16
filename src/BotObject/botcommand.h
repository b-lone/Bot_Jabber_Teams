#ifndef BOTCOMMAND_H
#define BOTCOMMAND_H

#include <QObject>
#include <memory>

class BotObject;

class BotCommand : public QObject
{
    Q_OBJECT
public:
    typedef std::shared_ptr<BotCommand> PTR;
    static PTR New(QJsonObject * jsonObject);

    //init by json
    BotCommand(QObject *parent = nullptr);

    void InitByJson(QJsonObject * jsonObject);

    QString resource;
    QString event;
    std::shared_ptr<BotObject> object;
};

#endif // BOTCOMMAND_H
