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
    explicit accWithdrawals(QByteArray& token, QWidget *parent = nullptr);
    ~accWithdrawals();

    // Public member function transTableWidget
    void transTableWidget(QStandardItemModel *model);

private:
    Ui::accWithdrawals *ui;

    QNetworkAccessManager *deleteManager;
    QNetworkReply *deleteReply;
    QByteArray deleteResponse_data;

    QByteArray webToken;

private slots:
    void handleBack();
    void deleteResource();
};

#endif // ACCWITHDRAWALS_H
