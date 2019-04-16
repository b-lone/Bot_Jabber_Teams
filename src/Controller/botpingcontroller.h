#ifndef BOTPINGCONTROLLER_H
#define BOTPINGCONTROLLER_H

#include <QObject>
#include <memory>

class QTimer;
class BotMessage;

class BotPingController : public QObject
{
    Q_OBJECT
public:
    explicit BotPingController(QObject *parent = nullptr);
    ~BotPingController();

    void Init(std::shared_ptr<BotMessage> msg);
    QString getRoomId();

    void StartPing(int timerInterval);//ms

private slots:
    void onTimer();

private:
    int count = 0;
    QTimer * timer;
    BotMessage *message;
};

#endif // BOTPINGCONTROLLER_H
