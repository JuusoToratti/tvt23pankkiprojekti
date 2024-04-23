#ifndef ACCOUNTBALANCE_H
#define ACCOUNTBALANCE_H

#include <QWidget>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

namespace Ui {
class accountBalance;
}

class accountBalance : public QWidget
{
    Q_OBJECT

public:
    explicit accountBalance(QByteArray& token, QWidget *parent = nullptr);
    ~accountBalance();

private:
    Ui::accountBalance *ui;

    QByteArray webToken;

private slots:
    void handleBackClicked();
    void handleLogoutClicked();  
};

#endif // ACCOUNTBALANCE_H
