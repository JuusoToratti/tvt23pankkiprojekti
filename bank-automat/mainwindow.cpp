#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cardreader.h" // Oletetaan, että CardReader-luokka vastaa kortinlukijan toiminnallisuudesta
#include "database.h" // Oletetaan, että Database-luokka vastaa tietokantatoiminnoista

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Luo kortinlukijaolio
    CardReader = new CardReader(this);
    // Yhdistää signaalit ja slotit kortinlukijaan
    connect(CardReader, &CardReader::cardInserted, this, &MainWindow::cardInserted);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete CardReader;
}

void MainWindow::cardInserted(const QString &cardID)
{
    // Avaa tunnuksen syöttö -käyttöliittymä
    enterPINWindow = new EnterPINWindow(this);
    // Yhdistää signaalit ja slotit tunnuksen syöttö -käyttöliittymään
    connect(enterPINWindow, &EnterPINWindow::pinEntered, this, &MainWindow::pinEntered);
    // Näytä tunnuksen syöttö -käyttöliittymä
    enterPINWindow->show();

    // Tallenna kortin ID
    currentCardID = cardID;
}

void MainWindow::pinEntered(const QString &pin)
{
    // Tarkista tunnusluku tietokannasta
    Database database;
    if (database.checkPIN(currentCardID, pin))
    {
        // Näytä pääkäyttöliittymä
        mainMenuWindow = new MainMenuWindow(currentCardID, this);
        mainMenuWindow->show();
        // Sulje tunnuksen syöttö -käyttöliittymä
        enterPINWindow->close();
        delete enterPINWindow;
    }
    else
    {
        // Ilmoita käyttäjälle virheellisestä tunnusluvusta
        QMessageBox::warning(this, "Virhe", "Virheellinen tunnusluku.");
        // Sulje tunnuksen syöttö -käyttöliittymä
        enterPINWindow->close();
        delete enterPINWindow;
        // Palaa ohjelman aloituskäyttöliittymään
        show();
    }
}
