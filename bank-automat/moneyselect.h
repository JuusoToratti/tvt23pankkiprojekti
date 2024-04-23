#ifndef MONEYSELECT_H
#define MONEYSELECT_H

#include <QWidget>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

QT_BEGIN_NAMESPACE
namespace Ui {
class moneySelect;
}
QT_END_NAMESPACE

class moneySelect : public QWidget
{
    Q_OBJECT

public:
    explicit moneySelect(QByteArray& token, QWidget *parent = nullptr);
    ~moneySelect();

private:
    Ui::moneySelect *ui;

    QNetworkAccessManager *getManager;
    QNetworkReply *getReply;
    QByteArray getResponse_data;

    QNetworkAccessManager *putManager;
    QNetworkReply *putReply;
    QByteArray putResponse_data;

    QNetworkAccessManager *postManager;
    QNetworkReply *postReply;
    QByteArray postResponse_data;

    QByteArray webToken;

private slots:
    void handleBackToMenu();
    void handleOtherAmount();

    void twentyEuroClickedPut();
    void put20Slot(QNetworkReply *putReply);
    void twentyEuroClickedGet();
    void twentyEuroClickedPost(QNetworkReply *getReply);
    void post20Slot(QNetworkReply *postReply);

    void fortyEuroClickedPut();
    void put40Slot(QNetworkReply *putReply);
    void fortyEuroClickedGet();
    void fortyEuroClickedPost(QNetworkReply *getReply);
    void post40Slot(QNetworkReply *postReply);

    void fiftyEuroClickedPut();
    void put50Slot(QNetworkReply *putReply);
    void fiftyEuroClickedGet();
    void fiftyEuroClickedPost(QNetworkReply *getReply);
    void post50Slot(QNetworkReply *postReply);

    void hundredEuroClickedPut();
    void put100Slot(QNetworkReply *putReply);
    void hundredEuroClickedGet();
    void hundredEuroClickedPost(QNetworkReply *getReply);
    void post100Slot(QNetworkReply *postReply);

    void insertHundredClickedPut();
    void putInsertedMoneySlot(QNetworkReply *putReply);
    void insertHundredClickedPost();
    void postInsertedMoneySlot(QNetworkReply *postReply);
};

#endif // MONEYSELECT_H
