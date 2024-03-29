#ifndef ADDPIN_H
#define ADDPIN_H

#include <QMainWindow>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class addPin;
}
QT_END_NAMESPACE

class addPin : public QMainWindow {
    Q_OBJECT

public:
    addPin(QWidget *parent = nullptr);
    ~addPin();

private:
    Ui::addPin *ui;
    const short correctPin = 5757;
    QString  enteredPin;

private slots:
    void handlePinInsert();
    void numberClickedHandler();
    void clearLineEdit();
};

#endif // ADDPIN_H
