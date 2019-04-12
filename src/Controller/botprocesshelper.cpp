#include "botprocesshelper.h"

#include <QProcess>
#include <QApplication>
#include <QSysInfo>

#include "botconfig.h"
#include "botcommon.h"

BotProcessHelper::BotProcessHelper(QObject *parent) : QObject(parent)
{
    process = new QProcess;
    QObject::connect(process, &QProcess::started, this, &BotProcessHelper::on_start);
    QObject::connect(process, &QProcess::errorOccurred, this, &BotProcessHelper::on_errorOccurred);
}

BotProcessHelper::~BotProcessHelper()
{
    if(process){
        process->kill();
        BOTLOG("Kill process:" << process->state());
    }
}

void BotProcessHelper::RunNgrok()
{
    if(process->isOpen()){
        BOTLOG("This process has already run.");
        return;
    }
    BOTLOG("Start ngrok.");

    auto os = QSysInfo().productType();
    QString ngrokPath;
    BOTLOG(os);
    if(os.contains("win")){
        ngrokPath = QApplication::applicationDirPath() + "/ngrok/ngrok.exe";
    }
    else if (os == "osx") {
        ngrokPath = QApplication::applicationDirPath() + "/ngrok/ngrok";
    }

    QStringList args;
    args << "http";
    args << S_CONFIG->Value(BotConfig::ngrokPort).toString();
    BOTLOG("ngrok parameters:" << args);

    process->start(ngrokPath, args);
    process->waitForStarted();
}

void BotProcessHelper::on_start()
{
    BOTLOG("Start process sucessful!");
}

void BotProcessHelper::on_errorOccurred()
{
    BOTLOG("Start process failed:" << process->error());
    emit errorOccurred("Start process failed!");
}
