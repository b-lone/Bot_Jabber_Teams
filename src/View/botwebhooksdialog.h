#ifndef BOTWEBHOOKSDIALOG_H
#define BOTWEBHOOKSDIALOG_H

#include <QDialog>

namespace Ui {
class BotWebhooksDialog;
}

enum RequestType : unsigned char;

class BotWebhooksDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BotWebhooksDialog(QWidget *parent = nullptr);
    ~BotWebhooksDialog();

public slots:
    void UpdateTable();

private slots:
    void on_btnRefresh_clicked();
    void on_btnActive_clicked();
    void on_btnDelete_clicked();
    void on_btnCreate_clicked();
    void on_btnGetUrl_clicked();
    void on_tableWebhooks_itemSelectionChanged();
    void update_leUrl(QString Url);
    void on_leUrl_textChanged(const QString &arg1);
    void on_deleteFinshed(RequestType type);

private:
    Ui::BotWebhooksDialog *ui;
};

#endif // BOTWEBHOOKSDIALOG_H
