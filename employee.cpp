#include "employee.h"
#include "ui_employee.h"
#include "mainwindow.h"

employee::employee(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::employee)
{
    ui->setupUi(this);
    //фоновая картинка
    QPixmap bkgnd("/home/student/Downloads/photo_5366529059481307011_x.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    app = new empapplications();
    connect(app,&empapplications::empWindowA,this,&guest::show);//сигнал на возврат на рабочее окно

    cust = new empcustomers;
    connect(cust,&empcustomers::empWindowC,this,&guest::show);//сигнал на возврат на рабочее окно

    man= new empmanagers();
    connect(man,&empmanagers::empWindowM,this,&guest::show);//сигнал на возврат на рабочее окно

    brig = new empbrigades();
    connect(brig,&empbrigades::empWindowB,this,&guest::show);//сигнал на возврат на рабочее окно

    work= new empworkers();
    connect(work,&empworkers::empWindowW,this,&guest::show);//сигнал на возврат на рабочее окно

    ono = new empono();
    connect(ono,&empono::empWindowO,this,&guest::show);//сигнал на возврат на рабочее окно



}











employee::~employee()
{
    delete ui;
}

//Обратно на форму авторизации
void employee::on_pushButton_clicked()
{
    this->close();
    emit firstWindow();
}

void employee::on_btnApplications_clicked()
{
    this->close();
    app->show();
}

void employee::on_btnCustomers_clicked()
{
    this->close();
    cust->show();
}

void employee::on_btnManagers_clicked()
{
    this->close();
    man->show();
}

void employee::on_btnBrigades_clicked()
{
    this->close();
    brig->show();
}

void employee::on_btnWorkers_clicked()
{
    this->close();
    work->show();
}

void employee::on_btnOno_clicked()
{
    this->close();
    ono->show();
}
