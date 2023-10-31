#ifndef GUESTALLOBJECTS_H
#define GUESTALLOBJECTS_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class guestAllObjects;
}

class guestAllObjects : public QDialog
{
    Q_OBJECT

public:
    explicit guestAllObjects(QWidget *parent = nullptr);
    ~guestAllObjects();
signals:
    void guestWindow();//сигнал для возврата на форму гостя

private slots:
    void on_btnBack_clicked();
    void on_pushButton_clicked();

private:
    Ui::guestAllObjects *ui;
    QSqlDatabase dbconn;
protected:
    void showEvent(QShowEvent *event) override;

public slots:
    void dbconnect();
    void selectAll();
};

#endif // GUESTALLOBJECTS_H
