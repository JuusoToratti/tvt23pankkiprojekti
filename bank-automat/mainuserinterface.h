#ifndef MAINUSERINTERFACE_H
#define MAINUSERINTERFACE_H

#include <QWidget>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class mainUserInterface;
}
QT_END_NAMESPACE

class mainUserInterface : public QWidget
{
    Q_OBJECT

public:
    explicit mainUserInterface(QWidget *parent = nullptr);
    ~mainUserInterface();

private:
    Ui::mainUserInterface *ui;

private slots:
   void logoutClicked();
    void withdrawMoneyClicked();
};

#endif // MAINUSERINTERFACE_H
