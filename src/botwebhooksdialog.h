#ifndef BOTWEBHOOKSDIALOG_H
#define BOTWEBHOOKSDIALOG_H

#include <QDialog>

namespace Ui {
class BotWebhooksDialog;
}

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

private:
    Ui::BotWebhooksDialog *ui;
};

#endif // BOTWEBHOOKSDIALOG_H
