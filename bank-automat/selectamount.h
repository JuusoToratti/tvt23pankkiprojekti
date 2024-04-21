#ifndef SELECTAMOUNT_H
#define SELECTAMOUNT_H

#include <QWidget>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

namespace Ui {
class selectAmount;
}

class selectAmount : public QWidget
{
    Q_OBJECT

public:
    explicit selectAmount(QWidget *parent = nullptr);
    ~selectAmount();

private:
    Ui::selectAmount *ui;
    QString enteredNum;

    QNetworkAccessManager *putManager;
    QNetworkReply *putReply;
    QByteArray putResponse_data;

    QNetworkAccessManager *postManager;
    QNetworkReply *postReply;
    QByteArray postResponse_data;

    double newAccountBalance;

private slots:
    void numClickedHandler();
    void clearLe();
    void backToMoneySelect();

    void putAmount();
    void putSelectAnyAmount (QNetworkReply *putReply);

    void postAmount();
    void postSelectAnyAmount(QNetworkReply *postReply);
};

#endif // SELECTAMOUNT_H
