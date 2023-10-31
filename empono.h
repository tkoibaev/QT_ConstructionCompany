#ifndef EMPONO_H
#define EMPONO_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class empono;
}

class empono : public QDialog
{
    Q_OBJECT

public:
    explicit empono(QWidget *parent = nullptr);
    ~empono();
protected:
    void showEvent(QShowEvent *event) override;
signals:
    void empWindowO();
private slots:
    void on_btnBack_clicked();
    void on_twOrg_cellClicked(int row, int column);
    void on_btnObj_clicked();

public slots:
    void dbconnect();
    void selectAll();
    //void leClear();
    //void delet();
    //void add();
private:
    Ui::empono *ui;
    QSqlDatabase dbconn;
};

#endif // EMPONO_H
