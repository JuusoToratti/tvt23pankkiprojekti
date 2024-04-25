#include "selectamount.h"
#include "ui_selectamount.h"
#include "moneyselect.h"
#include "ui_moneyselect.h"

selectAmount::selectAmount(QByteArray& token, QString cardNumber, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::selectAmount)
    , webToken(token)
    , cardNumber(cardNumber)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    // Create a QList called buttons and add buttons N0 to N9 to it
    QList<QPushButton*> buttons = {ui->N0, ui->N1, ui->N2, ui->N3, ui->N4, ui->N5, ui->N6, ui->N7, ui->N8, ui->N9};

    // Connect all the buttons to the same signal and call the numClickedHandler function
    for(QPushButton* button : buttons) {
        connect(button, &QPushButton::clicked, this, &selectAmount::numClickedHandler);
    }

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

    moneySelect *moneySelectWindow = new moneySelect(webToken, cardNumber);
    moneySelectWindow->show();
}

void selectAmount::putAmount()
{
    // Read the entered value
    QString enteredNum = ui->amountLe->text();
    int n = enteredNum.toInt();

    if (n % 5 != 0)
    {
        // Cannot withdraw from ATM
        ui->infoLabel->setText("Ei tarpeeksi oikeankokoisia seteleitä");
    } else {

    QJsonObject jsonObj;
    jsonObj.insert("account_balance",n );

    QString site_url="http://localhost:3000/accounts/update";
    QNetworkRequest request((site_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

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

    // Parse the response into JSON format
    QJsonDocument jsonResponse = QJsonDocument::fromJson(putResponse_data);
    QJsonObject jsonObject = jsonResponse.object();

    int changed=jsonObject["changedRows"].toInt();
    qDebug()<<"Changed="<<changed;

    if (changed == 0)
     {
      // The account balance would go negative, cannot withdraw money
      ui->infoLabel->setText("Tilillä ei ole tarpeeksi katetta");
     } else {
            // The withdrawal was successful, and the account balance remains positive
            ui->infoLabel->setText("Nosto onnistui");
            }
}

void selectAmount::getAmount()
{
    // Execute a GET request to retrieve the account balance
    QString site_url = "http://localhost:3000/accounts/getaccountbalance/1";
    QNetworkRequest getRequest(site_url);

    //WEBTOKEN START
    QByteArray myToken="Bearer "+webToken;
    getRequest.setRawHeader(QByteArray("Authorization"),(myToken));
    //WEBTOKEN END

    getManager = new QNetworkAccessManager(this);
    connect(getManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(postSelectAnyAmount(QNetworkReply*)));
    getReply = getManager->get(getRequest);
}

void selectAmount::postSelectAnyAmount(QNetworkReply *getReply)
{
    // POST-method

    // Ensure that the response is available and there is no error
        if (getReply->error() == QNetworkReply::NoError) {
        QByteArray response_data = getReply->readAll();
        qDebug() << "GET-pyynnön vastaus: " << response_data;

        // Handle the response in JSON format
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);

        if (json_doc.isObject()) {
            QJsonObject json_obj = json_doc.object();
            QString account_balance = json_obj.value("account_balance").toString();
            qDebug() << "Current account balance: " << account_balance;

        QString enteredNum = ui->amountLe->text();
        int n = enteredNum.toInt();

        if (n % 5 != 0)
        {
            // Cannot withdraw
            ui->infoLabel->setText("Ei tarpeeksi oikeankokoisia seteleitä");
        } else {
        // Check the account balance and make the POST request only if the balance is sufficient
        if (account_balance.toDouble() >= n) {

        QJsonObject postObj;
        postObj.insert("idaccount", 1); //tähän oikea arvo
        postObj.insert("transactions", "1"); //tähän oikea arvo
        postObj.insert("amount", -n);
        postObj.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate)); // Add current date

        QString post_url = "http://localhost:3000/accountinformation/create";
        QNetworkRequest postRequest(post_url);
        postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        //WEBTOKEN START
        QByteArray myToken="Bearer "+webToken;
        postRequest.setRawHeader(QByteArray("Authorization"),(myToken));
        //WEBTOKEN END

        postManager = new QNetworkAccessManager(this);
        connect(postManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(postAnyAmount(QNetworkReply*)));

        QJsonDocument postDoc(postObj);
        QByteArray postData = postDoc.toJson();

        postReply = postManager->post(postRequest, postData);
        } else {
            qDebug() << "Summaa ei voi nostaa";
          }
         }
        } else {
            qDebug() << "Virhe GET-pyynnön suorittamisessa: " << getReply->errorString();
        }
        // Release resources
        getReply->deleteLater();
        getManager->deleteLater();
 }
}

void selectAmount::postAnyAmount(QNetworkReply *postReply)
{
    postResponse_data=postReply->readAll();
    qDebug()<<postResponse_data;
    postReply->deleteLater();
    postManager->deleteLater();
}
