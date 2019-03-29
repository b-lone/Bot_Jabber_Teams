#ifndef BOTMAINWINDOW_H
#define BOTMAINWINDOW_H

#include <QMainWindow>
class QProcess;

namespace Ui {
class BotMainWindow;
}

class BotMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BotMainWindow(QWidget *parent = nullptr);
    ~BotMainWindow();
private:
    void runNgrok();

public slots:
    //ngrok Progress slot
    void on_start();
    void on_errorOccurred();

private slots:  
    void on_btnRooms_clicked();

    void on_btnMemberships_clicked();

    void on_btnAddMem_clicked();

    void on_btnMembership_clicked();

    void on_btnUpdateMem_clicked();

    void on_btnDeleteMem_clicked();

    void on_btnListMsg_clicked();

    void on_btnListDMsg_clicked();

    void on_btnCreateMsg_clicked();

    void on_btnGetMsg_clicked();

    void on_btnDeleteMsg_clicked();

    void on_btnListPeople_clicked();

    void on_btnGetPersonDetails_clicked();

    void on_btnCreateRoom_clicked();

    void on_btnGetRoomDetails_clicked();

    void on_btnUpdateRoom_clicked();

    void on_btnDeleteRoom_clicked();

    void on_btnListWebhooks_clicked();

    void on_btnCreateWebhook_clicked();

    void on_btnWebhookDetails_clicked();

    void on_btnUpdateWebhook_clicked();

    void on_btnDeleteWebhook_clicked();

    void on_btnNgrok_clicked();

private:
    Ui::BotMainWindow *ui;
    QProcess * ngrok;
};

#endif // BOTMAINWINDOW_H
