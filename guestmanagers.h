#ifndef GUESTMANAGERS_H
#define GUESTMANAGERS_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class guestManagers;
}

class guestManagers : public QDialog
{
    Q_OBJECT

public:
    explicit guestManagers(QWidget *parent = nullptr);
    ~guestManagers();

signals:
    void guestWindowM();//сигнал для возврата на форму гостя

private slots:
    void on_btnBack_clicked();

private:
    Ui::guestManagers *ui;
    QSqlDatabase dbconn;

public slots:
    void dbconnect();
    void selectAll();
};

#endif // GUESTMANAGERS_H
