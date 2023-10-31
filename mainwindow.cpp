#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employee.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->centralWidget()->setStyleSheet("background-image:url(\"/home/student/Downloads/photo_5366529059481307011_x.jpg\"); background-position:bottom;");

    emp_window = new employee();
    connect(emp_window,&employee::firstWindow,this,&MainWindow::show);//сигнал на возврат на главное окно

    guest_window = new guest();
    connect(guest_window,&guest::firstWindow,this,&MainWindow::show);//сигнал на возврат на главное окно

}

void MainWindow::on_btnLog_clicked()
{

    QString login = ui->leLogin->text();
    QString password = ui->lePassword->text();
    if((login=="e" || login=="Employee"|| login=="EMPLOYEE") && password=="123"){
        emp_window->show();
        this->close();
    } else if((login=="GUEST" || login=="guest"|| login=="Guest") && password=="321"){
        guest_window->show();
        this->close();
    } else {
        QMessageBox::warning(this,"Ошибка","Неверные данные");
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

