#include "moneyselect.h"
#include "ui_moneyselect.h"

moneySelect::moneySelect(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::moneySelect)
{
    ui->setupUi(this);
}

moneySelect::~moneySelect()
{
    delete ui;
}
