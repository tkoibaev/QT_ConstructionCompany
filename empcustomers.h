#ifndef EMPCUSTOMERS_H
#define EMPCUSTOMERS_H

#include <QDialog>
#include <QSqlDatabase>
namespace Ui {
class empcustomers;
}

class empcustomers : public QDialog
{
    Q_OBJECT

public:
    explicit empcustomers(QWidget *parent = nullptr);
    ~empcustomers();

signals:
    void empWindowC();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void on_btnBack_clicked();
    void on_btnFind_clicked();

    void on_twOrg_cellClicked(int row, int column);

    void on_btnDelete_clicked();

    void on_btnEdit_clicked();

    void on_pushButton_clicked();

    void on_btnInfo_clicked();

    void on_btnAdd_clicked();

public slots:
    void dbconnect();
    void selectAll();
    void delet();
    void leClear();
    void add();
private:
    Ui::empcustomers *ui;
    QSqlDatabase dbconn;
};

#endif // EMPCUSTOMERS_H
