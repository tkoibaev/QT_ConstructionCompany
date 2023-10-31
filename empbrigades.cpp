#include "empbrigades.h"
#include "ui_empbrigades.h"
#include "mainwindow.h"
#include <QSqlDatabase>
#include<QDebug>


extern int rowIndex1 = 1;

empbrigades::empbrigades(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::empbrigades)
{
    ui->setupUi(this);

    QPixmap bkgnd("/home/student/Downloads/photo_5366529059481307011_x.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    // Количество столбцов
    ui->twInfo->setColumnCount(5);
    // Возможность прокрутки
    ui->twInfo->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twInfo->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twInfo->setHorizontalHeaderItem(0,new QTableWidgetItem("ФИО"));
    ui->twInfo->setHorizontalHeaderItem(1,new QTableWidgetItem("Телефон"));
    ui->twInfo->setHorizontalHeaderItem(2,new QTableWidgetItem("Зарплата"));
    ui->twInfo->setHorizontalHeaderItem(3,new QTableWidgetItem("Должность"));
    ui->twInfo->setHorizontalHeaderItem(4,new QTableWidgetItem("Бригада"));

    //ui->twOrg->setHorizontalHeaderItem(3,new QTableWidgetItem("Количество объектов"));
    // Последний столбец растягивается при изменении размера формы
    ui->twInfo->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twInfo->setSortingEnabled(true);
    ui->twInfo->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);

}
void empbrigades::showEvent(QShowEvent *event) {
    selectAll();
}

void empbrigades::dbconnect()
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

void empbrigades::selectAll()
{
//    dbconn=QSqlDatabase::addDatabase("QPSQL");

//    dbconn.setDatabaseName("kurs_company");
    // Очистить содержимое компонента
    //ui->twOrg->clearContents();
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
    QString sqlstr = "select * from brigades";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
    QMessageBox::critical(this,"hihi", query.lastError().text());
    return;
    }
    // Если запрос активен (успешно завершен),
    // то вывести сообщение о прочитанном количестве строк в окно вывода
    // и установить количество строк для компонента таблицы
    if(query.first()) { //!!! ЗДЕСЬ ПЕРЕДАЕМ ФИО ИЗ БД В ПОЛЕ lineEdit !!!
        QString name = query.value("brigade_name").toString();
        QString boss = query.value("brigade_boss").toString();
        QString phone = query.value("brigade_phone").toString();
        ui->leName->setText(name);
        ui->leBoss->setText(boss);
        ui->lePhone->setText(phone);
    }

}

int empbrigades::countRows() {
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from brigades";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return 0;
    }

    int count = 0;
    while (query.next()) {
        count++;
    }
    return count;
}

bool empbrigades::checkPrevious(int ind) {
    if (--ind <= 0) {
        return false;
    }
    return true;
}

bool empbrigades::checkNext(int ind) {
    if (++ind > countRows()) {
        return false;
    }
    return true;
}

void empbrigades::next()
{
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from brigades";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    if (checkNext(rowIndex1)) {
        rowIndex1++;
        int i = 0;
        while (query.next()) {
            i++;
            if (i == rowIndex1) {
                QString name = query.value("brigade_name").toString();
                QString boss = query.value("brigade_boss").toString();
                QString phone = query.value("brigade_phone").toString();
                ui->leName->setText(name);
                ui->leBoss->setText(boss);
                ui->lePhone->setText(phone);
                ui->teResult->setText("Информация о палате выведена успешно!");
            }
        }
    }
    else {
        ui->teResult->setText("Больше палат в списке нет!");
    }
}

void empbrigades::previous()
{
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from brigades";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }

    if (checkPrevious(rowIndex1)) {
        rowIndex1--;
        int i = 0;
        while (query.next()) {
            i++;
            if (i == rowIndex1) {
                QString name = query.value("brigade_name").toString();
                QString boss = query.value("brigade_boss").toString();
                QString phone = query.value("brigade_phone").toString();
                ui->leName->setText(name);
                ui->leBoss->setText(boss);
                ui->lePhone->setText(phone);
                ui->teResult->setText("Информация о палате выведена успешно!");}
        }
    }
    else {
        ui->teResult->setText("Вы уже на первой записи списка!");
    }
}

void empbrigades::add(){

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
    QString sqlstr = "insert into brigades(brigade_id,brigade_name,brigade_boss,brigade_phone) values(?,?,?,?)";
    // Подготовить запрос
    query.prepare(sqlstr);

    //узнаем макс индекс среди имеющихся менеджеров
    QSqlQuery max;
    max.prepare("SELECT MAX(brigade_id) FROM brigades");
    max.exec();
    max.first();
    int i = max.value(0).toInt() + 1 ;
    QString rez = QString::number(i);

    // Передать параметры из полей ввода в запрос
    query.bindValue(0,rez);
    query.bindValue(1,ui->leName->text());
    query.bindValue(2,ui->leBoss->text());
    query.bindValue(3,ui->lePhone->text());

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

void empbrigades::delet(){
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
    //int currow = ui->twOrg->currentRow();
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
    QString sqlstr = "delete from brigades where brigade_name = '" + ui->leName->text() + "'";//111111111111111111111111111111
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
    ui->teResult->append( query.lastQuery());
    QMessageBox::critical(this,"Error",query.lastError().text());
    return;
    }
    // Вывести сообщение об удалении строки
    ui->teResult->append( QString("Del %1 rows").arg(query.numRowsAffected()) );
    // Обновить содержимое компонента таблицы
    selectAll();
}

void empbrigades::edit() {
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
    //int currow = ui->twOrg->currentRow();
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
    if( QMessageBox::question(this,"Update","Update row?",
    QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
    return;
    // Создать объект запроса
    QString name_up=ui->leName->text();
    QString phone_up=ui->lePhone->text();
    QString boss_up=ui->leBoss->text();

    QSqlQuery query(dbconn);
    // Создать строку запроса.
    // Вместо подготовки запроса и передачи параметров значение параметра
    // конкатенируется со строкой запроса
    // Обратите,что строковое значение помещается в одинарные кавычки
    // Значение выбирается из компонента таблицы методом item(row,col)
    QString sqlstr = "UPDATE brigades SET brigade_name='"+name_up+"',brigade_phone='"+phone_up+"',brigade_boss='"+boss_up+"' where brigade_name = '" + ui->leName->text() + "'";//111111111111111111111111111111


    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
    ui->teResult->append( query.lastQuery());
    QMessageBox::critical(this,"GEG",query.lastError().text());
    return;
    }
    // Вывести сообщение об удалении строки
    ui->teResult->append( QString("Update %1 rows").arg(query.numRowsAffected()) );
}

void empbrigades::info(){
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

    QString brigade=ui->leName->text();
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select workers.fio,workers.phone,workers.worker_salary,workers.worker_post,brigades.brigade_name from workers inner join brigades on workers.brig_id=brigades.brigade_id  where brigades.brigade_name='"+brigade+"'";
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
    QTableWidgetItem(query.value("workers.fio").toString()));
    ui->twInfo->setItem(i,1,new
    QTableWidgetItem(query.value("workers.phone").toString()));
    ui->twInfo->setItem(i,2,new
    QTableWidgetItem(query.value("workers.worker_salary").toString()));
    ui->twInfo->setItem(i,3,new
    QTableWidgetItem(query.value("workers.worker_post").toString()));
    ui->twInfo->setItem(i,4,new
    QTableWidgetItem(query.value("brigades.brigade_name").toString()));
    i++;
    }
}

empbrigades::~empbrigades()
{
    delete ui;
}

void empbrigades::on_btnBack_clicked()
{
    this->close();
    emit empWindowB();
}

void empbrigades::on_btnNext_clicked()
{
    next();
}

void empbrigades::on_btnPrevious_clicked()
{
    previous();
}

void empbrigades::on_btnAdd_clicked()
{
    add();
}

void empbrigades::on_btnDel_clicked()
{
    delet();
}

void empbrigades::on_btnEdit_clicked()
{
    edit();
}

void empbrigades::on_btnInfo_clicked()
{
    info();
}
