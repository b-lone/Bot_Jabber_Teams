#ifndef BOTSTORE_H
#define BOTSTORE_H

#include <QObject>
#include <QVector>

class BotRoom;
class BotMessage;

class BotStore : public QObject
{
    Q_OBJECT
public:
    static BotStore * Instance();

    void AddNewMessage(std::shared_ptr<BotMessage> message);

private:
    explicit BotStore(QObject *parent = nullptr);

signals:
   void NewMessage(std::shared_ptr<BotMessage>);

public slots:

private:
    QVector<std::shared_ptr<BotRoom>> roomStore;
    std::vector<std::shared_ptr<BotMessage>> messagesStore;
};

#define BOTSTORE BotStore::Instance()

#endif // BOTSTORE_H
