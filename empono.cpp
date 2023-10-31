#include "empono.h"
#include "ui_empono.h"
#include "mainwindow.h"
#include <QSqlDatabase>
#include<QDebug>

empono::empono(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::empono)
{
    ui->setupUi(this);

    // Количество столбцов
    ui->twOrg->setColumnCount(4);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("Номер заказа"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("Дата заказа"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("Заказчик"));
    ui->twOrg->setHorizontalHeaderItem(3,new QTableWidgetItem("Менеджер"));
    //ui->twOrg->setHorizontalHeaderItem(3,new QTableWidgetItem("Количество объектов"));
    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);


    // Количество столбцов
    ui->twInfo->setColumnCount(4);
    // Возможность прокрутки
    ui->twInfo->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twInfo->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twInfo->setHorizontalHeaderItem(0,new QTableWidgetItem("Название"));
    ui->twInfo->setHorizontalHeaderItem(1,new QTableWidgetItem("Стоимость"));
    ui->twInfo->setHorizontalHeaderItem(2,new QTableWidgetItem("Район"));
    ui->twInfo->setHorizontalHeaderItem(3,new QTableWidgetItem("Бригада"));
    //ui->twInfo->setHorizontalHeaderItem(4,new QTableWidgetItem("Бригада"));

    //ui->twOrg->setHorizontalHeaderItem(3,new QTableWidgetItem("Количество объектов"));
    // Последний столбец растягивается при изменении размера формы
    ui->twInfo->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twInfo->setSortingEnabled(true);
    ui->twInfo->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void empono::showEvent(QShowEvent *event) {
    selectAll();
}

void empono::dbconnect()
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

void empono::selectAll()
{
//    dbconn=QSqlDatabase::addDatabase("QPSQL");

//    dbconn.setDatabaseName("kurs_company");
    // Очистить содержимое компонента
    ui->twOrg->clearContents();
    // Если соединение не открыто, то вызвать нашу функцию для открытия
    // если подключиться не удалось, то вывести сообщение об ошибке и
    // выйти из функции
    dbconnect();
        if( !dbconn.isOpen() )
        {
        //dbconnect();
        if( !dbconn.isOpen() )
        {
        QMessageBox::critical(this,"haha",dbconn.lastError().text());
        return;
        }
        }
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select orders.order_id,orders.order_date,customers.fio,managers.fio from orders inner join managers on managers.manager_id=orders.man_id inner join customers on customers.customer_id=orders.cust_id order by orders.order_id desc";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
    QMessageBox::critical(this,"hihi", query.lastError().text());
    return;
    }
    // Если запрос активен (успешно завершен),
    // то вывести сообщение о прочитанном количестве строк в окно вывода
    // и установить количество строк для компонента таблицы
    if( query.isActive())
    ui->twOrg->setRowCount( query.size());
    else
    ui->twOrg->setRowCount( 0);
    //ui->teResult->append( QString("Найдено %1 ряда").arg(query.size()));
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
    ui->twOrg->setItem(i,0,new
    QTableWidgetItem(query.value("orders.order_id").toString()));
    ui->twOrg->setItem(i,1,new
    QTableWidgetItem(query.value("orders.order_date").toString()));
    ui->twOrg->setItem(i,2,new
    QTableWidgetItem(query.value("customers.fio").toString()));
    ui->twOrg->setItem(i,3,new
    QTableWidgetItem(query.value("managers.fio").toString()));
    i++;
    }
}






empono::~empono()
{
    delete ui;
}

void empono::on_btnBack_clicked()
{
    this->close();
    emit empWindowO();
}

void empono::on_twOrg_cellClicked(int row, int column)
{
    ui->leOrder->setText(ui->twOrg->item(row,0)->text());
    ui->leDate->setText(ui->twOrg->item(row,1)->text());
    ui->leCust->setText(ui->twOrg->item(row,2)->text());
    ui->leMan->setText(ui->twOrg->item(row,3)->text());

}

void empono::on_btnObj_clicked()
{
    // Очистить содержимое компонента
    ui->twInfo->clearContents();
    // Если соединение не открыто, то вызвать нашу функцию для открытия
    // если подключиться не удалось, то вывести сообщение об ошибке и
    // выйти из функции
    if( !dbconn.isOpen() )
    {
    dbconnect();
    if( !dbconn.isOpen() )
    {
    QMessageBox::critical(this,"Error",dbconn.lastError().text());
    return;
    }
    }

    QString order=ui->leOrder->text();
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select object_name,object_price,object_place,brigades.brigade_name from objects inner join brigades on brigades.brigade_id=objects.brig_id  where order_id='"+order+"'";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
    QMessageBox::critical(this,"Error", query.lastError().text());
    return;
    }
    // Если запрос активен (успешно завершен),
    // то вывести сообщение о прочитанном количестве строк в окно вывода
    // и установить количество строк для компонента таблицы
    if( query.size()<1)
        {
        QMessageBox::critical(this,"Такой бригады нет!", query.lastError().text());
        //ui->leFind->clear();
        //selectAll();
        return;
        }
    if( query.isActive())
    ui->twInfo->setRowCount( query.size());
    else
    ui->twInfo->setRowCount( 0);
    ui->teResult->append( QString("Найдено %1 ряда").arg(query.size()));
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
    ui->twInfo->setItem(i,0,new
    QTableWidgetItem(query.value("object_name").toString()));
    ui->twInfo->setItem(i,1,new
    QTableWidgetItem(query.value("object_price").toString()));
    ui->twInfo->setItem(i,2,new
    QTableWidgetItem(query.value("object_place").toString()));
    ui->twInfo->setItem(i,3,new
    QTableWidgetItem(query.value("brigades.brigade_name").toString()));
    i++;
    }
}
