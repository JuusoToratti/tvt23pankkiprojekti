#ifndef ACCWITHDRAWALS_H
#define ACCWITHDRAWALS_H

#include <QWidget>

namespace Ui {
class accWithdrawals;
}

class accWithdrawals : public QWidget
{
    Q_OBJECT

public:
    explicit accWithdrawals(QWidget *parent = nullptr);
    ~accWithdrawals();

private:
    Ui::accWithdrawals *ui;

private slots:
    void handleBack();
};

#endif // ACCWITHDRAWALS_H
