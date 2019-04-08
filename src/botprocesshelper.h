#ifndef BOTPROCESSHELPER_H
#define BOTPROCESSHELPER_H

#include <QObject>

class QProcess;

class BotProcessHelper : public QObject
{
    Q_OBJECT
public:
    explicit BotProcessHelper(QObject *parent = nullptr);
    ~BotProcessHelper();
    void RunNgrok();

signals:
    void errorOccurred(QString);

public slots:
    void on_start();
    void on_errorOccurred();

private:
    QProcess * process = nullptr;
};

#endif // BOTPROCESSHELPER_H
