#include "mainuserinterface.h"
#include "ui_mainuserinterface.h"
#include "moneyselect.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "accountbalance.h"
#include "ui_accountbalance.h"
#include "accwithdrawals.h"
#include "ui_accwithdrawals.h"
#include <QProcess>
#include <QStandardItem>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

mainUserInterface::mainUserInterface(QByteArray& token, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainUserInterface)
    , webToken(token)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    connect(ui->logOut, &QPushButton::clicked, this, &mainUserInterface::logoutClicked);
    connect(ui->withdrawMoney, &QPushButton::clicked, this, &mainUserInterface::withdrawMoneyClicked);
    connect(ui->showTransactions, &QPushButton::clicked, this, &mainUserInterface::handleTransactionsClicked);
    connect(ui->showBalance, &QPushButton::clicked, this , &mainUserInterface::getCreditBalanceSlot);
}

mainUserInterface::~mainUserInterface()
{
    delete ui;
}

void mainUserInterface::withdrawMoneyClicked()
{
    // Create and show moneySelect-object
    moneySelect *moneySelectWindow = new moneySelect(webToken);
    moneySelectWindow->show();

    // Close mainUserInterface-window
    this->close();
}

void mainUserInterface::handleTransactionsClicked()
{
    QString site_url="http://localhost:3000/accountinformation/info";
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    getManager = new QNetworkAccessManager(this);
    connect(getManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(transactionsNetworkReqFin(QNetworkReply*)));
    reply = getManager->get(request);
}

void mainUserInterface::transactionsNetworkReqFin(QNetworkReply *reply)
{
    //Account transactions

    QByteArray transResponse_data=reply->readAll();
    QJsonDocument json_doc = QJsonDocument::fromJson(transResponse_data);
    QJsonArray json_array = json_doc.array();

    QStandardItemModel *table_model = new QStandardItemModel(json_array.size(),2);
    //table_model->setHeaderData(0, Qt::Horizontal, QObject::tr("Tilinumero"));
    //table_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Noston numero"));
    table_model->setHeaderData(0, Qt::Horizontal, QObject::tr("Määrä"));
    table_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Päiväys"));

    for (int row = 0; row < json_array.size(); ++row) {
        QJsonObject json_obj = json_array[row].toObject(); // Määritellään json_obj tässä
       // QStandardItem *idaccount = new QStandardItem(QString::number(json_obj["idaccount"].toInt()));
       // table_model->setItem(row, 0, idaccount);
       // QStandardItem *transactions = new QStandardItem(json_obj["transactions"].toString());
        //table_model->setItem(row, 1, transactions);
        QStandardItem *amount = new QStandardItem(json_obj["amount"].toString());
        table_model->setItem(row, 0, amount);
        QStandardItem *date = new QStandardItem(json_obj["date"].toString());
        table_model->setItem(row, 1, date);
    }

    // Find the accwithdrawals window and set the model to its transTableWidget
    accWithdrawals *accWithdrawalsWindow = new accWithdrawals(webToken);
    accWithdrawalsWindow->transTableWidget(table_model);
    accWithdrawalsWindow->show();

    // Close mainUserInterface-window
    this->close();

    reply->deleteLater();
    getManager->deleteLater();
}

void mainUserInterface::getCreditBalanceSlot()
{
    //If the URL changes on the REST API side, then there should also be a corresponding change here
    QString site_url="http://localhost:3000/accounts/getaccountbalance/1";
    QNetworkRequest request((site_url));

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    pgetManager = new QNetworkAccessManager(this);
    connect(pgetManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onNetworkRequestFinished(QNetworkReply*)));
    preply = pgetManager->get(request);
}

void mainUserInterface::onNetworkRequestFinished(QNetworkReply *preply)
{
    //Debit balance

    QByteArray response_data=preply->readAll();
    qDebug()<<"DATA : "+response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);

    if (json_doc.isObject()) {
        QJsonObject json_obj = json_doc.object();
        QString account_balance = json_obj.value("account_balance").toString();
        qDebug() << "Current account balance: " << account_balance;

    // Create a new window and a user interface object
    accountBalance *accountBalanceWindow = new accountBalance(webToken);

    QLabel *creditLabel = accountBalanceWindow->findChild<QLabel*>("creditLabel");
    if (creditLabel) {
        creditLabel->setText(account_balance);
    }
    accountBalanceWindow->show();

    // Close the current window
    this->close();

    preply->deleteLater();
    pgetManager->deleteLater();
 }
}

void mainUserInterface::logoutClicked()
{
    // Close the app
    qApp->quit();

    // Restart the app
    QProcess::startDetached(QApplication::applicationFilePath());
}
