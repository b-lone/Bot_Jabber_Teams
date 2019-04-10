#ifndef BOTMAINWINDOW_H
#define BOTMAINWINDOW_H

#include <QMainWindow>
#include <memory>
class QProcess;
class BotNetworkController;
class BotProcessHelper;

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

private slots:  
    void on_btnWebhookDialog_clicked();

private:
    Ui::BotMainWindow *ui;
    BotProcessHelper *processHelper;
};

#endif // BOTMAINWINDOW_H
