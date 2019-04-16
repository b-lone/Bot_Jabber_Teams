#ifndef BOTCOMMANDCONTROLLER_H
#define BOTCOMMANDCONTROLLER_H

#include <QObject>
#include <memory>
class BotCommand;

class BotCommandController : public QObject
{
    Q_OBJECT
public:
    explicit BotCommandController(QObject *parent = nullptr);

private:
    void ProcessMessages(std::shared_ptr<BotCommand> object);
    void ProcessMessagesCreated(std::shared_ptr<BotCommand> object);

public slots:
    void on_commandReady(std::shared_ptr<BotCommand> object);
};

#endif // BOTCOMMANDCONTROLLER_H
