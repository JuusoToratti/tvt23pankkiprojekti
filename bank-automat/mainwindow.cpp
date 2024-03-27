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

#include <QMainWindow>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //tähän rfid-kortti
    if (RFIDsimulation == 8080)
    {
        // Luo uusi ikkuna ja käyttöliittymäolio
        QMainWindow *addPinWindow = new QMainWindow();
        Ui::addPin *addPinUi = new Ui::addPin();

        // Aseta käyttöliittymä addPin-ikkunaan
        addPinUi->setupUi(addPinWindow);

        // Näytä ikkuna
        addPinWindow->show();

        this->close();

        delete addPinUi;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
