#ifndef ADDPIN_H
#define ADDPIN_H

#include <QMainWindow>
#include <QLineEdit>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

QT_BEGIN_NAMESPACE
namespace Ui {
class addPin;
}
QT_END_NAMESPACE

class addPin : public QWidget{
    Q_OBJECT

public:
    addPin(QWidget *parent = nullptr);
    ~addPin();

private:
    Ui::addPin *ui;
    const short correctPin = 5757;
    QString  enteredPin;
    QTimer *timer;

private slots:
    void handlePinInsert();
    void numberClickedHandler();
    void clearLineEdit();
    void timerTimeout();


protected:
    void closeEvent(QCloseEvent *event);
};

#endif // ADDPIN_H
