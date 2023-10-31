#ifndef EMPAPPLICATIONS_H
#define EMPAPPLICATIONS_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class empapplications;
}

class empapplications : public QDialog
{
    Q_OBJECT

public:
    explicit empapplications(QWidget *parent = nullptr);
    ~empapplications();
signals:
    void empWindowA();

protected:
    void showEvent(QShowEvent *event) override;


private slots:
    void on_btnBack_clicked();

    void on_twOrg_cellClicked(int row, int column);

    void on_btnDelete_clicked();

    void on_btnEdit_clicked();

    void on_btnCust_clicked();

    void on_btnFind_clicked();

public slots:
    void dbconnect();
    void selectAll();
    void delet();

private:
    Ui::empapplications *ui;
    QSqlDatabase dbconn;
};

#endif // EMPAPPLICATIONS_H
