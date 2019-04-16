#ifndef BOTMAINWINDOW_H
#define BOTMAINWINDOW_H

#include <QMainWindow>
#include <memory>
class QProcess;
class BotHttpClientController;
class BotProcessHelper;
class BotWebhookPushController;
class BotCommandController;

namespace Ui {
class BotMainWindow;
}

class BotMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BotMainWindow(QWidget *parent = nullptr);
    ~BotMainWindow();

public slots:
   void showMessages(QString msg);
   void reboot();

private slots:  
    void on_btnWebhookDialog_clicked();

    void on_btnSetting_clicked();

private:
    Ui::BotMainWindow *ui;
    BotProcessHelper *processHelper;
    BotWebhookPushController *webhookPushController;
    BotCommandController *commandController;
};

#endif // BOTMAINWINDOW_H
