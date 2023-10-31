#include "guest.h"
#include "ui_guest.h"
#include "mainwindow.h"
#include <QSqlDatabase>

guest::guest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::guest)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/photo_5366529059481307011_x.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    //this->centralWidget()->setStyleSheet("background-image:url(\"/home/student/Downloads/photo_5366529059481307011_x.jpg\"); background-position:bottom;");

    allObjects = new guestAllObjects();
    connect(allObjects,&guestAllObjects::guestWindow,this,&guest::show);//сигнал на возврат на гостевое окно

    managers = new guestManagers();
    connect(managers,&guestManagers::guestWindowM,this,&guest::show);//сигнал на возврат на гостевое окно

    brigade = new guestBrigades();
    connect(brigade,&guestBrigades::guestWindowB,this,&guest::show);//сигнал на возврат на гостевое окно

    application = new guestApplications();
    connect(application,&guestApplications::guestWindowA,this,&guest::show);//сигнал на возврат на гостевое окно

}

guest::~guest()
{
    delete ui;

}


//Обратно на форму авторизации
void guest::on_btnBack_clicked()
{
    this->close();
    emit firstWindow();
}

//Список объектов(первая кнопка)
void guest::on_btnOurObjects_clicked()
{
    this->close();
    allObjects->show();
}

//Список менеджеров(вторая кнопка)
void guest::on_btnManagers_clicked()
{
    this->close();
    managers->show();
}

//Список бригад(третья кнопка)
void guest::on_btnBrigades_clicked()
{
    this->close();
    brigade->show();
}

void guest::on_btnApplication_clicked()
{
    this->close();
    application->show();
}
