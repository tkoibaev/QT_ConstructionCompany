#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QDialog>
#include<empapplications.h>
#include<empcustomers.h>
#include<empmanagers.h>
#include<empbrigades.h>
#include<empworkers.h>
#include<empono.h>

namespace Ui {
    class employee;
}

class employee : public QDialog
{
    Q_OBJECT

public:
    explicit employee(QWidget *parent = nullptr);
    ~employee();

signals:
    void firstWindow();

private slots:
    void on_pushButton_clicked();

    void on_btnApplications_clicked();

    void on_btnCustomers_clicked();

    void on_btnManagers_clicked();

    void on_btnBrigades_clicked();

    void on_btnWorkers_clicked();

    void on_btnOno_clicked();

private:
    Ui::employee *ui;
    empapplications *app;
    empcustomers *cust;
    empmanagers *man;
    empbrigades *brig;
    empworkers *work;
    empono *ono;

};

#endif // EMPLOYEE_H
