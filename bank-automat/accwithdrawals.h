#ifndef ACCWITHDRAWALS_H
#define ACCWITHDRAWALS_H

#include <QWidget>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QStandardItem>


namespace Ui {
class accWithdrawals;
}

class accWithdrawals : public QWidget
{
    Q_OBJECT

public:
    explicit accWithdrawals(QWidget *parent = nullptr);
    ~accWithdrawals();

    // Julkinen jäsenfunktio transTableWidget
    void transTableWidget(QStandardItemModel *model);

private:
    Ui::accWithdrawals *ui;

    QNetworkAccessManager *deleteManager;
    QNetworkReply *deleteReply;
    QByteArray deleteResponse_data;

private slots:
    void handleBack();
    void deleteResource();
};

#endif // ACCWITHDRAWALS_H
