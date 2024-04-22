#include "accwithdrawals.h"
#include "ui_accwithdrawals.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItem>

accWithdrawals::accWithdrawals(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::accWithdrawals)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    // Luo uusi QNetworkAccessManager-olio
    deleteManager = new QNetworkAccessManager(this);

    connect(ui->withBackButton, &QPushButton::clicked, this, &accWithdrawals::handleBack);
    connect(ui->deleteActionsButton, &QPushButton::clicked, this, &accWithdrawals::deleteResource);

    // Aseta transTableWidget
    transTableWidget(new QStandardItemModel(this));
}

accWithdrawals::~accWithdrawals()
{
    delete ui;
}

void accWithdrawals::transTableWidget(QStandardItemModel *model)
{
    ui->transTableWidget->setModel(model);
}

void accWithdrawals::handleBack()
{
    // Luo uusi ikkuna ja käyttöliittymäolio
    mainUserInterface *mainUserInterfaceWindow = new mainUserInterface();
    mainUserInterfaceWindow->show();

    //suljetaan nykyinen ikkuna
    this->close();
}

void accWithdrawals::deleteResource()
{
    // Luodaan DELETE-pyyntö
    QUrl url("http://localhost:3000/accountinformation/delete");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Tee DELETE-pyyntö
    QNetworkReply *deleteReply = deleteManager->deleteResource(request);

    // Käsitellään vastaus, kun se saapuu
    connect(deleteReply, &QNetworkReply::finished, this, [=]() {
        if (deleteReply->error() == QNetworkReply::NoError) {
            // Vastaus onnistui
            qDebug() << "DELETE-pyyntö onnistui";
            // Tyhjennä transTableWidget
            QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->transTableWidget->model());
            if (model) {
                model->clear();
            }
        } else {
            // Vastaus epäonnistui
            qDebug() << "Virhe DELETE-pyynnön suorittamisessa:" << deleteReply->errorString();
        }
        // Vapautetaan resurssit
        deleteReply->deleteLater();
    });
}
