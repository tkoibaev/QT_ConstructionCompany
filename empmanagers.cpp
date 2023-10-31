#include "empmanagers.h"
#include "ui_empmanagers.h"
#include "mainwindow.h"
#include <QSqlDatabase>
#include<QDebug>


empmanagers::empmanagers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::empmanagers)
{
    ui->setupUi(this);

    QPixmap bkgnd("/home/student/Downloads/photo_5366529059481307011_x.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    // Количество столбцов
    ui->twOrg->setColumnCount(4);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("ФИО"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("Телефон"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("еmail"));
    ui->twOrg->setHorizontalHeaderItem(3,new QTableWidgetItem("Зарплата"));
    //ui->twOrg->setHorizontalHeaderItem(3,new QTableWidgetItem("Количество объектов"));
    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void empmanagers::showEvent(QShowEvent *event) {
    selectAll();
}

void empmanagers::dbconnect()
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

void empmanagers::selectAll()
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
    QString sqlstr = "select * from managers";
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
    ui->twOrg->setItem(i,3,new
    QTableWidgetItem(query.value("salary").toString()));
    i++;
    }
}


empmanagers::~empmanagers()
{
    delete ui;
}

void empmanagers::on_btnBack_clicked()
{
    this->close();
    emit empWindowM();
    leClear();
}

void empmanagers::delet(){
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
    QString sqlstr = "delete from managers where fio = '" + ui->twOrg->item(currow,0)->text() + "'";//111111111111111111111111111111
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
    ui->teResult->append( query.lastQuery());
    QMessageBox::critical(this,"haha","Этот менеджер имеет открытые сделки! Сперва удалите их!");
    return;
    }
    // Вывести сообщение об удалении строки
    ui->teResult->append( QString("Del %1 rows").arg(query.numRowsAffected()) );
    // Обновить содержимое компонента таблицы

    leClear();
    selectAll();
}

void empmanagers::add(){

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
    QString sqlstr = "insert into managers(manager_id,fio,phone,email,salary) values(?,?,?,?,?)";
    // Подготовить запрос
    query.prepare(sqlstr);

    //узнаем макс индекс среди имеющихся менеджеров
    QSqlQuery max;
    max.prepare("SELECT MAX(manager_id) FROM managers");
    max.exec();
    max.first();
    int i = max.value(0).toInt() + 1 ;
    QString rez = QString::number(i);

    // Передать параметры из полей ввода в запрос
    query.bindValue(0,rez);
    query.bindValue(1,ui->leFIO->text());
    query.bindValue(2,ui->lePhone->text());
    query.bindValue(3,ui->leEmail->text());
    query.bindValue(4,ui->leSalary->text());

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

void empmanagers::on_twOrg_cellClicked(int row, int column)
{
    int currow = row;
    ui->leFIO->setText(ui->twOrg->item(currow,0)->text());
    ui->lePhone->setText(ui->twOrg->item(currow,1)->text());
    ui->leEmail->setText(ui->twOrg->item(currow,2)->text());
    ui->leSalary->setText(ui->twOrg->item(currow,3)->text());
}

void empmanagers::leClear(){
    ui->leFind->clear();
    ui->leFIO->clear();
    ui->lePhone->clear();
    ui->leEmail->clear();
    ui->leSalary->clear();
    //ui->twInfo->clear();
}

void empmanagers::on_btnDelete_clicked()
{
    delet();
}

void empmanagers::on_btnEdit_clicked()
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
    QString salary_up=ui->leSalary->text();

    QSqlQuery query(dbconn);
    // Создать строку запроса.
    // Вместо подготовки запроса и передачи параметров значение параметра
    // конкатенируется со строкой запроса
    // Обратите,что строковое значение помещается в одинарные кавычки
    // Значение выбирается из компонента таблицы методом item(row,col)
    QString sqlstr = "UPDATE managers SET fio='"+fio_up+"',phone='"+phone_up+"',email='"+email_up+"',salary='"+salary_up+"' where fio = '" + ui->twOrg->item(currow,0)->text() + "'";//111111111111111111111111111111


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

void empmanagers::on_pushButton_clicked()
{
    selectAll();
    leClear();
}

void empmanagers::on_btnFind_clicked()
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
    QString sqlstr = "select fio,phone,email,salary from managers where fio ILIKE '%' ||'"+app+"'|| '%'";
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
        QMessageBox::critical(this,"Такого менеджера нет!", query.lastError().text());
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
    ui->twOrg->setItem(i,3,new
    QTableWidgetItem(query.value("salary").toString()));
    i++;
    }
}

void empmanagers::on_btnAdd_clicked()
{
    add();
}
