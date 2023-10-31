#include "empworkers.h"
#include "ui_empworkers.h"
#include "mainwindow.h"
#include <QSqlDatabase>
#include<QDebug>

empworkers::empworkers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::empworkers)
{
    ui->setupUi(this);


    QPixmap bkgnd("/home/student/Downloads/photo_5366529059481307011_x.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);


    // Количество столбцов
    ui->twOrg->setColumnCount(5);
    // Возможность прокрутки
    ui->twOrg->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twOrg->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twOrg->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twOrg->setHorizontalHeaderItem(0,new QTableWidgetItem("ФИО"));
    ui->twOrg->setHorizontalHeaderItem(1,new QTableWidgetItem("Должность"));
    ui->twOrg->setHorizontalHeaderItem(2,new QTableWidgetItem("Телефон"));
    ui->twOrg->setHorizontalHeaderItem(3,new QTableWidgetItem("Зарплата"));
    ui->twOrg->setHorizontalHeaderItem(4,new QTableWidgetItem("Бригада"));

    //ui->twOrg->setHorizontalHeaderItem(3,new QTableWidgetItem("Количество объектов"));
    // Последний столбец растягивается при изменении размера формы
    ui->twOrg->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twOrg->setSortingEnabled(true);
    ui->twOrg->sortByColumn(0);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twOrg->setEditTriggers(QAbstractItemView::NoEditTriggers);

}
void empworkers::showEvent(QShowEvent *event) {
    selectAll();
}

void empworkers::dbconnect()
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

void empworkers::selectAll()
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

    QString sqlstr1 = "select brigade_name from brigades";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr1) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }

    QComboBox *brigade_ptr = ui->cbBrigade;
    while(query.next())
    {
        brigade_ptr->addItem(query.value("brigade_name").toString());
    }

    // Создать строку запроса на выборку данных
    QString sqlstr = "select workers.fio,workers.worker_post,workers.phone,workers.worker_salary,brigades.brigade_name from workers inner join brigades on workers.brig_id=brigades.brigade_id";
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
    QTableWidgetItem(query.value("workers.fio").toString()));
    ui->twOrg->setItem(i,1,new
    QTableWidgetItem(query.value("workers.worker_post").toString()));
    ui->twOrg->setItem(i,2,new
    QTableWidgetItem(query.value("workers.phone").toString()));
    ui->twOrg->setItem(i,3,new
    QTableWidgetItem(query.value("workers.worker_salary").toString()));
    ui->twOrg->setItem(i,4,new
    QTableWidgetItem(query.value("brigades.brigade_name").toString()));
    i++;
    }
}

void empworkers::add(){

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

    // Создать строку запроса на выборку данных
    QString sqlstr2 = "select brigade_id, brigade_name from brigades";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr2) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }

    int currentBrigadeId;
    while(query.next())
    {
        if (ui->cbBrigade->currentText() == query.value("brigade_name").toString()) {
            currentBrigadeId = query.value("brigade_id").toInt();
        }
    }

    // Создать строку запроса
    QString sqlstr = "insert into workers(worker_id,fio,phone,worker_salary,worker_post,brig_id) values(?,?,?,?,?,?)";
    // Подготовить запрос
    query.prepare(sqlstr);

    //узнаем макс индекс среди имеющихся менеджеров
    QSqlQuery max;
    max.prepare("SELECT MAX(worker_id) FROM workers");
    max.exec();
    max.first();
    int i = max.value(0).toInt() + 1 ;
    QString rez = QString::number(i);

    // Передать параметры из полей ввода в запрос
    query.bindValue(0,rez);
    query.bindValue(1,ui->leFIO_2->text());
    query.bindValue(2,ui->lePhone_2->text());
    query.bindValue(3,ui->leSalary_2->text());
    query.bindValue(4,ui->lePost_2->text());
    query.bindValue(5,currentBrigadeId);


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

void empworkers::leClear() {
    ui->leFIO->clear();
    ui->lePost->clear();
    ui->lePhone->clear();
    ui->leSalary->clear();
    ui->leFIO_2->clear();
    ui->lePost_2->clear();
    ui->lePhone_2->clear();
    ui->leSalary_2->clear();
}

empworkers::~empworkers()
{
    delete ui;
}

void empworkers::on_btnBack_clicked()
{
    ui->leFind->clear();
    this->close();
    emit empWindowW();
}

void empworkers::on_btnFind_clicked()
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

    QString name=ui->leFind->text();
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select workers.fio,workers.worker_post,workers.phone,workers.worker_salary,brigades.brigade_name from workers inner join brigades on workers.brig_id=brigades.brigade_id where workers.fio ILIKE '%' ||'"+name+"'|| '%'";
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
        QTableWidgetItem(query.value("workers.fio").toString()));
        ui->twOrg->setItem(i,1,new
        QTableWidgetItem(query.value("workers.worker_post").toString()));
        ui->twOrg->setItem(i,2,new
        QTableWidgetItem(query.value("workers.phone").toString()));
        ui->twOrg->setItem(i,3,new
        QTableWidgetItem(query.value("workers.worker_salary").toString()));
        ui->twOrg->setItem(i,4,new
        QTableWidgetItem(query.value("brigades.brigade_name").toString()));
        i++;
    }
}

void empworkers::on_pushButton_clicked()
{
    ui->leFind->clear();
    selectAll();
}

void empworkers::on_twOrg_cellClicked(int row, int column)
{
    int currow = row;
    ui->leFIO->setText(ui->twOrg->item(currow,0)->text());
    ui->lePost->setText(ui->twOrg->item(currow,1)->text());
    ui->lePhone->setText(ui->twOrg->item(currow,2)->text());
    ui->leSalary->setText(ui->twOrg->item(currow,3)->text());
    //ui->leBrigade->setText(ui->twOrg->item(currow,4)->text());

}

void empworkers::on_btnEdit_clicked()
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
    QString post_up=ui->lePost->text();
    QString salary_up=ui->leSalary->text();
    //QString salary_up=ui->leSalary->text();

    QSqlQuery query(dbconn);
    // Создать строку запроса.
    // Вместо подготовки запроса и передачи параметров значение параметра
    // конкатенируется со строкой запроса
    // Обратите,что строковое значение помещается в одинарные кавычки
    // Значение выбирается из компонента таблицы методом item(row,col)
    QString sqlstr = "UPDATE workers SET fio='"+fio_up+"',phone='"+phone_up+"',worker_salary='"+salary_up+"',worker_post='"+post_up+"' where fio = '" + ui->twOrg->item(currow,0)->text() + "'";//111111111111111111111111111111


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

void empworkers::on_btnAdd_clicked()
{
    add();

    leClear();
}

void empworkers::delet(){
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
    QString sqlstr = "delete from workers where fio = '" + ui->twOrg->item(currow,0)->text() + "'";//111111111111111111111111111111
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

void empworkers::on_btnDelete_clicked()
{
    delet();
    leClear();
}
