#include "selectamount.h"
#include "ui_selectamount.h"
#include "moneyselect.h"
#include "ui_moneyselect.h"

selectAmount::selectAmount(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::selectAmount)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    // Connect signals and slots
    connect(ui->N0, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N1, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N2, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N3, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N4, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N5, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N6, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N7, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N8, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    connect(ui->N9, &QPushButton::clicked, this, &selectAmount::numClickedHandler);

    connect(ui->wipe, &QPushButton::clicked, this, &selectAmount::clearLe);
    connect(ui->backToMs,  &QPushButton::clicked, this, &selectAmount::backToMoneySelect);

    connect(ui->withdrawSa, &QPushButton::clicked, this, &selectAmount::putAmount);
    connect(ui->withdrawSa, &QPushButton::clicked, this, &selectAmount::getAmount);
}

selectAmount::~selectAmount()
{
    delete ui;
}

void selectAmount::numClickedHandler()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString number = button->text();
        ui->amountLe->setText(ui->amountLe->text() + number);
    }
}

void selectAmount::clearLe()
{
    ui->amountLe->clear();
}

void selectAmount::backToMoneySelect()
{
    close();

    // Avataan pääikkuna (mainwindow.ui)
    moneySelect *moneySelectWindow = new moneySelect();
    moneySelectWindow->show();
}

void selectAmount::putAmount()
{
    // Luetaan syötetty arvo
    QString enteredNum = ui->amountLe->text();
    int n = enteredNum.toInt();

    if (n % 5 != 0)
    {
        // Ei voi nostaa
        ui->infoLabel->setText("Ei tarpeeksi oikeankokoisia seteleitä");
    } else {

    QJsonObject jsonObj;
    jsonObj.insert("account_balance",n );

    QString site_url="http://localhost:3000/accounts/update";
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    /*WEBTOKEN ALKU
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    WEBTOKEN */

    putManager = new QNetworkAccessManager(this);
    connect(putManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(putSelectAnyAmount(QNetworkReply*)));

    putReply = putManager->put(request, QJsonDocument(jsonObj).toJson());
    }
 }

void selectAmount::putSelectAnyAmount(QNetworkReply *putReply)
{
    putResponse_data=putReply->readAll();
    qDebug()<<putResponse_data;
    putReply->deleteLater();
    putManager->deleteLater();

    // Analysoi vastaus JSON-muotoon
    QJsonDocument jsonResponse = QJsonDocument::fromJson(putResponse_data);
    QJsonObject jsonObject = jsonResponse.object();

    int changed=jsonObject["changedRows"].toInt();
    qDebug()<<"Changed="<<changed;

    if (changed == 0)
     {
      // Tilin saldo menisi miinukselle, ei voida nostaa rahaa
      ui->infoLabel->setText("Summaa ei voi nostaa");
     } else {
            // Nosto onnistui ja tilin saldo pysyy positiivisena
            ui->infoLabel->setText("Nosto onnistui");
            }
}

void selectAmount::getAmount()
{
    // Suorita GET-pyyntö tilin saldon hakemiseksi
    QString site_url = "http://localhost:3000/accounts/getaccountbalance";
    QNetworkRequest getRequest(site_url);

    getManager = new QNetworkAccessManager(this);
    connect(getManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(postSelectAnyAmount(QNetworkReply*)));
    getReply = getManager->get(getRequest);
}

void selectAmount::postSelectAnyAmount(QNetworkReply *getReply)
{
    // POST-metodi

    // Varmista, että vastaus on saatavilla ja ei ole tapahtunut virhettä
        if (getReply->error() == QNetworkReply::NoError) {
        QByteArray response_data = getReply->readAll();
        qDebug() << "GET-pyynnön vastaus: " << response_data;

        // Käsittele vastaus JSON-muodossa
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
        QJsonArray json_array = json_doc.array();

        QString cCredit;

        foreach (const QJsonValue &value, json_array) {
            QJsonObject json_obj = value.toObject();
            QString account_balance = json_obj["account_balance"].toString();
            cCredit = account_balance;
            qDebug() << "Account balance: " << cCredit;
        }

        QString enteredNum = ui->amountLe->text();
        int n = enteredNum.toInt();

        if (n % 5 != 0)
        {
            // Ei voi nostaa
            ui->infoLabel->setText("Ei tarpeeksi oikeankokoisia seteleitä");
        } else {
        // Tarkista tilin saldo ja tee POST-pyyntö vain, jos saldo riittää
        if (cCredit.toDouble() >= n) {

        QJsonObject postObj;
        postObj.insert("idaccount", 1); //tähän oikea arvo
        postObj.insert("transactions", "1"); //tähän oikea arvo
        postObj.insert("amount", -n);
        postObj.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate)); // Lisää nykyinen päivämäärä

        QString post_url = "http://localhost:3000/accountinformation/create";
        QNetworkRequest postRequest(post_url);
        postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        postManager = new QNetworkAccessManager(this);
        connect(postManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(postAnyAmount(QNetworkReply*)));

        QJsonDocument postDoc(postObj);
        QByteArray postData = postDoc.toJson();

        postReply = postManager->post(postRequest, postData);
        } else {
            qDebug() << "Tilillä ei ole tarpeeksi katetta";
            // Tähän voit lisätä tarvittavan logiikan, esim. käyttäjälle näytettävän virheilmoituksen
          }
         }
        } else {
            qDebug() << "Virhe GET-pyynnön suorittamisessa: " << getReply->errorString();
            // Tähän voit lisätä tarvittavan virheen käsittelyn
        }

        // Vapauta resurssit
        getReply->deleteLater();
        getManager->deleteLater();
}

void selectAmount::postAnyAmount(QNetworkReply *postReply)
{
    postResponse_data=postReply->readAll();
    qDebug()<<postResponse_data;
    postReply->deleteLater();
    postManager->deleteLater();
}
