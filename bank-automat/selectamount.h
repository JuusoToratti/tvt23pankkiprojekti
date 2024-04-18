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
    QNetworkReply *reply;
    QByteArray response_data;

    double newAccountBalance;

private slots:
    void numClickedHandler();
    void clearLe();
    void backToMoneySelect();
    void checkAmount();

    void selectAnyAmount (QNetworkReply *reply);
};

#endif // SELECTAMOUNT_H
