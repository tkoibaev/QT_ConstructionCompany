#include "someselect.h"
#include "ui_someselect.h"

someselect::someselect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::someselect)
{
    ui->setupUi(this);
}

someselect::~someselect()
{
    delete ui;
}
