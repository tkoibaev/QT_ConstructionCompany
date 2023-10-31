#include "guestallobjects.h"
#include "ui_guestallobjects.h"
#include "mainwindow.h"
#include <QSqlDatabase>

guestAllObjects::guestAllObjects(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::guestAllObjects)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/student/Downloads/photo_5366529059481307011_x.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    //this->centralWidget()->setStyleSheet("background-image:url(\"/home/student/Downloads/photo_5366529059481307011_x.jpg\"); background-position:bottom;");


    //connect(ui->btnConnect,SIGNAL(clicked(bool)),this, SLOT(dbconnect()));
    connect(ui->btnSelectAll,SIGNAL(clicked(bool)),this, SLOT(selectAll()));


    // Количество столбцов
    ui->twOrg->setColumnCount(3);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("Название"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("Район"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("Стоймость"));
    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

//Событие при открытии формы
void guestAllObjects::showEvent(QShowEvent *event) {
    selectAll();
}

//Подключение к БД
void guestAllObjects::dbconnect()
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

//Select всех записей
void guestAllObjects::selectAll()
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
    QString sqlstr = "select * from objects";
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
    QTableWidgetItem(query.value("object_name").toString()));
    ui->twOrg->setItem(i,1,new
    QTableWidgetItem(query.value("object_place").toString()));
    ui->twOrg->setItem(i,2,new
    QTableWidgetItem(query.value("object_price").toString()));
    i++;
    }
}


//Выход в меню гостя
void guestAllObjects::on_btnBack_clicked()
{
    this->close();
    emit guestWindow();
}

//Поиск по районам
void guestAllObjects::on_pushButton_clicked()
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

    QString place=ui->leFind->text();
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from objects where object_place='"+place+"'";
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
        QMessageBox::critical(this,"Объектов в этом районе нет!", query.lastError().text());
        ui->leFind->clear();
        selectAll();
        return;
        }
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
    QTableWidgetItem(query.value("object_name").toString()));
    ui->twOrg->setItem(i,1,new
    QTableWidgetItem(query.value("object_place").toString()));
    ui->twOrg->setItem(i,2,new
    QTableWidgetItem(query.value("object_price").toString()));
    i++;
    }
    ui->leFind->clear();
}


guestAllObjects::~guestAllObjects()
{
    delete ui;
}
