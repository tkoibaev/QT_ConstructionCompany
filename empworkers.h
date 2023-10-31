#ifndef EMPWORKERS_H
#define EMPWORKERS_H

#include <QDialog>
#include <QSqlDatabase>
namespace Ui {
class empworkers;
}

class empworkers : public QDialog
{
    Q_OBJECT

public:
    explicit empworkers(QWidget *parent = nullptr);
    ~empworkers();

signals:
    void empWindowW();

private slots:
    void on_btnBack_clicked();

    void on_btnFind_clicked();

    void on_pushButton_clicked();

    void on_twOrg_cellClicked(int row, int column);

    void on_btnEdit_clicked();

    void on_btnAdd_clicked();

    void on_btnDelete_clicked();

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::empworkers *ui;
    QSqlDatabase dbconn;

public slots:
    void dbconnect();
    void selectAll();
    void leClear();
    void add();
    void delet();
};

#endif // EMPWORKERS_H
