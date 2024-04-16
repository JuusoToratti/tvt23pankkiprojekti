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
    explicit moneySelect(QWidget *parent = nullptr);
    ~moneySelect();

private:
    Ui::moneySelect *ui;

    QNetworkAccessManager *putManager;
    QNetworkReply *reply;
    QByteArray response_data;

private slots:
    void handleBackToMenu();
    void handleOtherAmount();

    void twentyEuroClickedSlot();
    void update20Slot(QNetworkReply *reply);

    void fortyEuroClickedSlot();
    void update40Slot(QNetworkReply *reply);

    void fiftyEuroClickedSlot();
    void update50Slot(QNetworkReply *reply);

    void hundredEuroClickedSlot();
    void update100Slot(QNetworkReply *reply);
};

#endif // MONEYSELECT_H
