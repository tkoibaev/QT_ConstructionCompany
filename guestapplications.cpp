#include "guestapplications.h"
#include "ui_guestapplications.h"
#include "mainwindow.h"
#include <QSqlDatabase>

guestApplications::guestApplications(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::guestApplications)
{
    ui->setupUi(this);

    QPixmap bkgnd("/home/student/Downloads/photo_5366529059481307011_x.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    //connect(ui->btnConnect,SIGNAL(clicked(bool)),this, SLOT(dbconnect()));

}

void guestApplications::dbconnect()
{
if(!dbconn.isOpen())
{
    ui->teResult->append("SQL drivers:");
    ui->teResult->append(QSqlDatabase::drivers().join(","));

    dbconn=QSqlDatabase::addDatabase("QPSQL");

    dbconn.setDatabaseName("kurs_company");
    //dbconn.setHostName("localhost");

    if( dbconn.open() )
        ui->teResult->append("Соединение готово...");
    else
    {
        ui->teResult->append("Ошибка соединения:");
        ui->teResult->append(dbconn.lastError().text());
        }
    }
    else
    ui->teResult->append("Соединение уже открыто...");
}


void guestApplications::on_btnBack_clicked()
{
    this->close();
    emit guestWindowA();
}
guestApplications::~guestApplications()
{
    delete ui;
}



void guestApplications::on_btnCheck_clicked()
{
    // Подключение к БД
    if( !dbconn.isOpen() )
    {
    dbconnect();
    if( !dbconn.isOpen() )
    {
    QMessageBox::critical(this,"Error",dbconn.lastError().text());
    return;
    }
    }
    // Получить номер выбранной строки в компоненте таблицы
//    int currow = ui->twOrg->currentRow();
    // Если он меньше 0 (строка не выбрана), то
    // сообщение об ошибке и выход из функции
//    if( currow < 0 )
//    {
//    QMessageBox::critical(this,"Error","Not selected row!");
//    return;
//    }
    // Спросить у пользователя подтверждение удаления записи
    // Используется статический метод QMessageBox::question
    // для задания вопроса, который возвращает код нажатой кнопки
//    if( QMessageBox::question(this,"Update","Update row?",
//    QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
//    return;
    // Создать объект запроса
    QString FIO=ui->leFIO->text();
    QString phone=ui->lePhone->text();
    QString email=ui->leEmail->text();

    QSqlQuery query(dbconn);
    // Создать строку запроса.
    // Вместо подготовки запроса и передачи параметров значение параметра
    // конкатенируется со строкой запроса
    // Обратите,что строковое значение помещается в одинарные кавычки
    // Значение выбирается из компонента таблицы методом item(row,col)
    QString sqlstr = "insert into applications(fio,phone,email) values ('"+FIO+"','"+phone+"','"+email+"')";//111111111111111111111111111111


    //Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
    ui->teResult->append( query.lastQuery());
    QMessageBox::critical(this,"GEG",query.lastError().text());
    return;
    }
    //Вывести сообщение об удалении строки
    ui->teResult->append( QString("Update %1 rows").arg(query.numRowsAffected()) );
    //Обновить содержимое компонента таблицы
    //selectAll();
}
