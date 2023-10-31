#include "empcustomers.h"
#include "ui_empcustomers.h"
#include "mainwindow.h"
#include <QSqlDatabase>
#include<QDebug>

empcustomers::empcustomers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::empcustomers)
{
    ui->setupUi(this);

    QPixmap bkgnd("/home/student/Downloads/photo_5366529059481307011_x.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    // Количество столбцов
    ui->twOrg->setColumnCount(3);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("ФИО"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("Телефон"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("еmail"));
    //ui->twOrg->setHorizontalHeaderItem(3,new QTableWidgetItem("Количество объектов"));
    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);




    // Количество столбцов
    ui->twInfo->setColumnCount(5);
    // Возможность прокрутки
    ui->twInfo->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twInfo->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twInfo->setHorizontalHeaderItem(0,new QTableWidgetItem("Заказчик"));
    ui->twInfo->setHorizontalHeaderItem(1,new QTableWidgetItem("Номер заказа"));
    ui->twInfo->setHorizontalHeaderItem(2,new QTableWidgetItem("Дата заказа"));
    ui->twInfo->setHorizontalHeaderItem(3,new QTableWidgetItem("Менеджер сделки"));
    ui->twInfo->setHorizontalHeaderItem(4,new QTableWidgetItem("Объект"));
    //ui->twOrg->setHorizontalHeaderItem(3,new QTableWidgetItem("Количество объектов"));
    // Последний столбец растягивается при изменении размера формы
    ui->twInfo->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twInfo->setSortingEnabled(true);
    ui->twInfo->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void empcustomers::showEvent(QShowEvent *event) {
    selectAll();
} //здесь можно сразу при открытии формы подключиться к БД


void empcustomers::dbconnect()
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


void empcustomers::selectAll()
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
    QString sqlstr = "select * from customers";
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
    QTableWidgetItem(query.value("fio").toString()));
    ui->twOrg->setItem(i,1,new
    QTableWidgetItem(query.value("phone").toString()));
    ui->twOrg->setItem(i,2,new
    QTableWidgetItem(query.value("email").toString()));
    i++;
    }
}

void empcustomers::delet(){
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
    int currow = ui->twOrg->currentRow();
    // Если он меньше 0 (строка не выбрана), то
    // сообщение об ошибке и выход из функции
    if( currow < 0 )
    {
    QMessageBox::critical(this,"Error","Not selected row!");
    return;
    }
    // Спросить у пользователя подтверждение удаления записи
    // Используется статический метод QMessageBox::question
    // для задания вопроса, который возвращает код нажатой кнопки
    if( QMessageBox::question(this,"Delete","Ряд будет удален!",
    QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
    return;
    // Создать объект запроса
    QSqlQuery query(dbconn);
    // Создать строку запроса.
    // Вместо подготовки запроса и передачи параметров значение параметра
    // конкатенируется со строкой запроса
    // Обратите,что строковое значение помещается в одинарные кавычки
    // Значение выбирается из компонента таблицы методом item(row,col)
    QString sqlstr = "delete from customers where fio = '" + ui->twOrg->item(currow,0)->text() + "'";//111111111111111111111111111111
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
    ui->teResult->append( query.lastQuery());
    QMessageBox::critical(this,"haha","Этот заказчик имеет откртые сделки! Сперва удалите их!");
    return;
    }
    // Вывести сообщение об удалении строки
    ui->teResult->append( QString("Del %1 rows").arg(query.numRowsAffected()) );
    // Обновить содержимое компонента таблицы

    leClear();
    selectAll();
}

void empcustomers::leClear(){
    ui->leFind->clear();
    ui->leFIO->clear();
    ui->lePhone->clear();
    ui->leEmail->clear();
    ui->twInfo->clearContents();
}


empcustomers::~empcustomers()
{
    delete ui;
}

void empcustomers::on_btnBack_clicked()
{
    this->close();
    emit empWindowC();
    leClear();
}

void empcustomers::on_btnFind_clicked()
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

    QString app=ui->leFind->text();
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select fio,phone,email from customers where fio ILIKE '%' ||'"+app+"'|| '%'";
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
        QMessageBox::critical(this,"Такой заявки нет!", query.lastError().text());
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
    QTableWidgetItem(query.value("fio").toString()));
    ui->twOrg->setItem(i,1,new
    QTableWidgetItem(query.value("phone").toString()));
    ui->twOrg->setItem(i,2,new
    QTableWidgetItem(query.value("email").toString()));
    i++;
    }
}


void empcustomers::on_twOrg_cellClicked(int row, int column)
{
    int currow = row;
    ui->leFIO->setText(ui->twOrg->item(currow,0)->text());
    ui->lePhone->setText(ui->twOrg->item(currow,1)->text());
    ui->leEmail->setText(ui->twOrg->item(currow,2)->text());
}

void empcustomers::on_btnDelete_clicked()
{
    delet();
}

void empcustomers::on_btnEdit_clicked()
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
    int currow = ui->twOrg->currentRow();
    // Если он меньше 0 (строка не выбрана), то
    // сообщение об ошибке и выход из функции
    if( currow < 0 )
    {
    QMessageBox::critical(this,"Error","Not selected row!");
    return;
    }
    // Спросить у пользователя подтверждение удаления записи
    // Используется статический метод QMessageBox::question
    // для задания вопроса, который возвращает код нажатой кнопки
    if( QMessageBox::question(this,"Update","Update row?",
    QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
    return;
    // Создать объект запроса
    QString fio_up=ui->leFIO->text();
    QString phone_up=ui->lePhone->text();
    QString email_up=ui->leEmail->text();

    QSqlQuery query(dbconn);
    // Создать строку запроса.
    // Вместо подготовки запроса и передачи параметров значение параметра
    // конкатенируется со строкой запроса
    // Обратите,что строковое значение помещается в одинарные кавычки
    // Значение выбирается из компонента таблицы методом item(row,col)
    QString sqlstr = "UPDATE customers SET fio='"+fio_up+"',phone='"+phone_up+"',email='"+email_up+"' where fio = '" + ui->twOrg->item(currow,0)->text() + "'";//111111111111111111111111111111


    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
    ui->teResult->append( query.lastQuery());
    QMessageBox::critical(this,"GEG",query.lastError().text());
    return;
    }
    // Вывести сообщение об удалении строки
    ui->teResult->append( QString("Update %1 rows").arg(query.numRowsAffected()) );
    // Обновить содержимое компонента таблицы
    leClear();
    selectAll();
}

void empcustomers::on_pushButton_clicked()
{
    selectAll();
    leClear();
}

void empcustomers::on_btnInfo_clicked()
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

    QString cust=ui->leFIO->text();
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select customers.fio,orders.order_id,orders.order_date,managers.fio,objects.object_name from customers inner join orders ON orders.cust_id=customers.customer_id inner join managers ON orders.man_id=managers.manager_id inner join objects ON objects.order_id=orders.order_id  where customers.fio='"+cust+"'";
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
    QTableWidgetItem(query.value("customers.fio").toString()));
    ui->twInfo->setItem(i,1,new
    QTableWidgetItem(query.value("orders.order_id").toString()));
    ui->twInfo->setItem(i,2,new
    QTableWidgetItem(query.value("orders.order_date").toString()));
    ui->twInfo->setItem(i,3,new
    QTableWidgetItem(query.value("managers.fio").toString()));
    ui->twInfo->setItem(i,4,new
    QTableWidgetItem(query.value("objects.object_name").toString()));
    i++;
    }
}

void empcustomers::add(){

    // Подключиться к БД
    if( !dbconn.isOpen() )
    {
    dbconnect();
    if( !dbconn.isOpen() )
    {
    QMessageBox::critical(this,"Error",dbconn.lastError().text());
    return;
    }
    }
    QSqlQuery query(dbconn);

    // Создать строку запроса
    QString sqlstr = "insert into customers(customer_id,fio,phone,email) values(?,?,?,?)";
    // Подготовить запрос
    query.prepare(sqlstr);

    //узнаем макс индекс среди имеющихся менеджеров
    QSqlQuery max;
    max.prepare("SELECT MAX(customer_id) FROM customers");
    max.exec();
    max.first();
    int i = max.value(0).toInt() + 1 ;
    QString rez = QString::number(i);

    // Передать параметры из полей ввода в запрос
    query.bindValue(0,rez);
    query.bindValue(1,ui->leFIO->text());
    query.bindValue(2,ui->lePhone->text());
    query.bindValue(3,ui->leEmail->text());

    // Если тип поля отличается от строкового, то преобразовать его
    //query.bindValue(3,ui->leInn->text().toLongLong());
    // Выполнить запрос
    if( !query.exec() )
    {
    ui->teResult->append( query.lastQuery());
    QMessageBox::critical(this,"Error",query.lastError().text());
    return;
    }
    // Если запрос выполнен, то вывести сообщение одобавлении строки
    ui->teResult->append( QString("AddRead %1rows").arg(query.numRowsAffected()) );
    // и обновить записи в компоненте таблицы
    selectAll();
}


void empcustomers::on_btnAdd_clicked()
{
    add();
}
