#ifndef SELECTAMOUNT_H
#define SELECTAMOUNT_H

#include <QWidget>

namespace Ui {
class selectAmount;
}

class selectAmount : public QWidget
{
    Q_OBJECT

public:
    explicit selectAmount(QWidget *parent = nullptr);
    ~selectAmount();

private:
    Ui::selectAmount *ui;
    QString enteredNum;

private slots:
    void handleInsertedNum();
    void numClickedHandler();
    void clearLe();
    void backToMoneySelect();
    void checkAmount();
};

#endif // SELECTAMOUNT_H
