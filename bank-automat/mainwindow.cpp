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

    //tähän rfid-kortti
    if (RFIDsimulation == 8080)
    {
        this->close();
        // Luo uusi ikkuna ja käyttöliittymäolio
        addPin *addPinWindow = new addPin();
        addPinWindow->show();

    } else {
        ui->begin->setText("Korttia ei tunnistettu");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
