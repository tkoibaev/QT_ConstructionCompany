#include "guestbrigades.h"
#include "ui_guestbrigades.h"
#include "mainwindow.h"
#include <QSqlDatabase>

guestBrigades::guestBrigades(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::guestBrigades)
{
    ui->setupUi(this);

    QPixmap bkgnd("/home/student/Downloads/photo_5366529059481307011_x.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    //connect(ui->btnConnect,SIGNAL(clicked(bool)),this, SLOT(dbconnect()));
    //connect(ui->btnSelectAll,SIGNAL(clicked(bool)),this, SLOT(selectAll()));

    // Количество столбцов
    ui->twOrg->setColumnCount(3);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("Название"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("Бригадир"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("Связь"));
    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);


    // Количество столбцов
    ui->twOrgFind->setColumnCount(3);
    // Возможность прокрутки
    ui->twOrgFind->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrgFind->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrgFind->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrgFind->setHorizontalHeaderItem(0,new QTableWidgetItem("ФИО"));
    ui->twOrgFind->setHorizontalHeaderItem(1,new QTableWidgetItem("Телефон"));
    ui->twOrgFind->setHorizontalHeaderItem(2,new QTableWidgetItem("Должность"));

    // Последний столбец растягивается при изменении размера формы
    ui->twOrgFind->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrgFind->setSortingEnabled(true);
    ui->twOrgFind->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrgFind->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void guestBrigades::showEvent(QShowEvent *event) {
    selectAll();
}

void guestBrigades::dbconnect()
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

//Список всех бригад
void guestBrigades::selectAll()
{
    // Очистить содержимое компонента
    ui->twOrg->clearContents();
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
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select brigade_name,brigade_boss,brigade_phone from brigades";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
    QMessageBox::critical(this,"Error", query.lastError().text());
    return;
    }
    // Если запрос активен (успешно завершен),
    // то вывести сообщение о прочитанном количестве строк в окно вывода
    // и установить количество строк для компонента таблицы
    if( query.isActive())
    ui->twOrg->setRowCount( query.size());
    else
    ui->twOrg->setRowCount( 0);
    ui->teResult->append( QString("Найдено %1 ряда").arg(query.size()));
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
    ui->twOrg->setItem(i,0,new
    QTableWidgetItem(query.value("brigade_name").toString()));
    ui->twOrg->setItem(i,1,new
    QTableWidgetItem(query.value("brigade_boss").toString()));
    ui->twOrg->setItem(i,2,new
    QTableWidgetItem(query.value("brigade_phone").toString()));
    i++;
    }
}

//Список рабочих в требуемой бригаде
void guestBrigades::on_btnFind_clicked(bool checked)
{
    // Очистить содержимое компонента
    ui->twOrgFind->clearContents();
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

    QString brigade=ui->leFind->text();
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select fio,phone,worker_post from workers inner join brigades on workers.brig_id=brigades.brigade_id where brigade_name='"+brigade+"'";
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
        ui->leFind->clear();
        //selectAll();
        return;
        }
    if( query.isActive())
    ui->twOrgFind->setRowCount( query.size());
    else
    ui->twOrgFind->setRowCount( 0);
    ui->teResult->append( QString("Найдено %1 ряда").arg(query.size()));
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
    ui->twOrgFind->setItem(i,0,new
    QTableWidgetItem(query.value("fio").toString()));
    ui->twOrgFind->setItem(i,1,new
    QTableWidgetItem(query.value("phone").toString()));
    ui->twOrgFind->setItem(i,2,new
    QTableWidgetItem(query.value("worker_post").toString()));
    i++;
    }
}

void guestBrigades::on_btnBack_clicked()
{
    this->close();
    emit guestWindowB();
}
guestBrigades::~guestBrigades()
{
    delete ui;
}




