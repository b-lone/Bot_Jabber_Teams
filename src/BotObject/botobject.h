#ifndef BOTOBJECT_H
#define BOTOBJECT_H

#include <QObject>

#include <memory>

class BotObject : public QObject
{
    Q_OBJECT
public:
    typedef  std::shared_ptr<BotObject> PTR;
    
    BotObject(QObject *parent = nullptr);

    virtual ~BotObject() = default;

    virtual QString Describe() = 0;

    QString id;
};

QDebug operator <<(QDebug qd, BotObject & object);

#endif // BOTOBJECT_H
