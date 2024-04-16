#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainUserInterface.h"
#include "ui_mainuserinterface.h"

#include "addPin.h"
#include "ui_addPin.h"

#include "ui_moneySelect.h"

#include <QTimer>
#include <QMessageBox>
#include <QDialog>
#include <QDebug>

#include <QMainWindow>

#include "rfidreaderdll.h"

#include "cdchoice.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("C:/bank.background.jpg")));
    this->setPalette(palette);

    // Luo RFID-lukijan olio
    RFIDReaderdll *rfidReader = new RFIDReaderdll(this);

    // Yhdistä signaali ja slot
    connect(rfidReader, &RFIDReaderdll::cardDetected, this, &MainWindow::handleCardDetected); 
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleCardDetected(QString cardID)
{
    // Tunnista kortin ID ja avaa oikea ikkuna sen perusteella
    qDebug() << "Kortista luetut tiedot:" << cardID;

    if (cardID == "-0600062093\r\n>") {

        cardNumber = cardID;
        //suljetaan nykyinen ikkuna
        addPin *addPinWindow = new addPin(cardNumber);
        this->close();
        addPinWindow->show();

    } else if (cardID == "-06000621FE\r\n>") {

        cardNumber = cardID;
        // Avaa ikkuna 2
        ui->begin->setText("CD-kortti");
        cdChoice * cdChoiceWindow = new cdChoice(cardNumber);
        this->close();
        cdChoiceWindow->show();
    } else {
        ui->begin->setText("Korttia ei tunnistettu");
    }
  }
