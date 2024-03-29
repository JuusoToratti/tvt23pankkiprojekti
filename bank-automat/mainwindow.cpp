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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Luodaan mainUserInterface-olio ja yhdistetään signaali ja slot
    userInterface = new mainUserInterface();
    connect(userInterface, &mainUserInterface::backToMainwindow, this, &MainWindow::onMainUserInterfaceClosed);


    //tähän rfid-kortti
    if (RFIDsimulation == 8080)
    {
        // Luo uusi ikkuna ja käyttöliittymäolio
        addPin *addPinWindow = new addPin();
        addPinWindow->show();

        this->close();
    }
}

void MainWindow::onMainUserInterfaceClosed()
{
    qDebug() << "onMainUserInterfaceClosedissa";
    this->show(); // Näytetään pääikkuna, kun mainUserInterface suljetaan
}

MainWindow::~MainWindow()
{
    delete ui;
}
