#ifndef ADDPIN_H
#define ADDPIN_H

#include <QMainWindow>
#include <QLineEdit>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

QT_BEGIN_NAMESPACE
namespace Ui {
class addPin;
}
QT_END_NAMESPACE

class addPin : public QWidget{
    Q_OBJECT

public:
    explicit addPin(QWidget *parent = nullptr);
    ~addPin();

private:
    Ui::addPin *ui;
    QString  enteredPin;
    QTimer *timer;
    QString correctPin = "";
    int cardtype = 0;

    QNetworkAccessManager *pgetManager;
    QNetworkReply *preply;
    QByteArray response_data;

    QNetworkAccessManager *pgetManagerPin;
    QNetworkReply *preplyPin;
    QByteArray response_dataPin;

private slots:
    void handlePinInsert();
    void numberClickedHandler();
    void clearLineEdit();
    void timerTimeout();

    void getNamesSlot (QNetworkReply *preply);
    void getPinSlot (QNetworkReply *preplyPin);

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // ADDPIN_H
