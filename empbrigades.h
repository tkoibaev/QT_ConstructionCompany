#ifndef EMPBRIGADES_H
#define EMPBRIGADES_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class empbrigades;
}

class empbrigades : public QDialog
{
    Q_OBJECT

public:
    explicit empbrigades(QWidget *parent = nullptr);
    ~empbrigades();
signals:
    void empWindowB();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void on_btnBack_clicked();

    void on_btnNext_clicked();

    void on_btnPrevious_clicked();

    void on_btnAdd_clicked();

    void on_btnDel_clicked();

    void on_btnEdit_clicked();

    void on_btnInfo_clicked();

public slots:
    void dbconnect();
    void selectAll();
    int countRows();
    bool checkNext(int ind);
    bool checkPrevious(int ind);
    void next();
    void previous();
    void add();
    void delet();
    void edit();
    void info();

private:
    Ui::empbrigades *ui;
    QSqlDatabase dbconn;
};

#endif // EMPBRIGADES_H
