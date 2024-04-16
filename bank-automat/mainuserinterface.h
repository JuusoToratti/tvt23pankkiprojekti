#ifndef MAINUSERINTERFACE_H
#define MAINUSERINTERFACE_H

#include <QWidget>
#include <QMainWindow>
#include <mainwindow.h>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

QT_BEGIN_NAMESPACE
namespace Ui {
class mainUserInterface;
}
QT_END_NAMESPACE

class mainUserInterface : public QWidget
{
    Q_OBJECT

public:
    explicit mainUserInterface(QWidget *parent = nullptr);
    ~mainUserInterface();

private:
    Ui::mainUserInterface *ui;

    QNetworkAccessManager *pgetManager;
    QNetworkReply *preply;
    QByteArray response_data;

private slots:
   void logoutClicked();
   void withdrawMoneyClicked();
   void handleTransactionsClicked();

   void getCreditBalanceSlot ();
   void onNetworkRequestFinished(QNetworkReply *preply);
};

#endif // MAINUSERINTERFACE_H
