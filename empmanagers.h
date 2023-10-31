#ifndef EMPMANAGERS_H
#define EMPMANAGERS_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class empmanagers;
}

class empmanagers : public QDialog
{
    Q_OBJECT

public:
    explicit empmanagers(QWidget *parent = nullptr);
    ~empmanagers();

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void empWindowM();

private slots:
    void on_btnBack_clicked();
    void on_twOrg_cellClicked(int row, int column);

    void on_btnDelete_clicked();

    void on_btnEdit_clicked();

    void on_pushButton_clicked();

    void on_btnFind_clicked();

    void on_btnAdd_clicked();

public slots:
    void dbconnect();
    void selectAll();
    void leClear();
    void delet();
    void add();
private:
    Ui::empmanagers *ui;
    QSqlDatabase dbconn;
};

#endif // EMPMANAGERS_H
