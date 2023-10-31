#ifndef GUESTAPPLICATIONS_H
#define GUESTAPPLICATIONS_H

#include <QDialog>
#include <QSqlDatabase>
namespace Ui {
class guestApplications;
}

class guestApplications : public QDialog
{
    Q_OBJECT

public:
    explicit guestApplications(QWidget *parent = nullptr);
    ~guestApplications();

signals:
    void guestWindowA();//сигнал для возврата на форму гостя

private slots:
    void on_btnBack_clicked();
    void on_btnCheck_clicked();

private:
    Ui::guestApplications *ui;
    QSqlDatabase dbconn;

public slots:
    void dbconnect();
    //void on_btnCheck_clicked();
};

#endif // GUESTAPPLICATIONS_H
