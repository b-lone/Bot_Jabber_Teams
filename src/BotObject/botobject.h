#ifndef BOTOBJECT_H
#define BOTOBJECT_H

#include <QObject>

class BotObject : public QObject
{
    Q_OBJECT
public:
    explicit BotObject(QObject *parent = nullptr);

    virtual ~BotObject() = default;

    void setId(const QString & Id){ id = Id; }
    const QString & getId(){ return id; }
protected:
    QString id;
};

#endif // BOTOBJECT_H
