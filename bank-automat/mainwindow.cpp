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
    qDebug() << "Lätkästä luetut tiedot:" << cardID;

    if (cardID == "-0600062093\r\n>") {

        //suljetaan nykyinen ikkuna
        addPin *addPinWindow = new addPin();
        this->close();
        addPinWindow->show();

    } else if (cardID == "-06000621FE\r\n>") {
        // Avaa ikkuna 2
        ui->begin->setText("CD-kortti");
        cdChoice * cdChoiceWindow = new cdChoice();
        this->close();
        cdChoiceWindow->show();
    } else {
        ui->begin->setText("Korttia ei tunnistettu");
    }
  }
