#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include "mainuserinterface.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onMainUserInterfaceClosed(); // Slot mainUserInterface:n sulkemiselle

private:
    Ui::MainWindow *ui;
    const short RFIDsimulation = 8080;
    mainUserInterface *userInterface; // Lisätty mainUserInterface -olio
};

#endif // MAINWINDOW_H
