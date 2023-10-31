#ifndef GUESTBRIGADES_H
#define GUESTBRIGADES_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class guestBrigades;
}

class guestBrigades : public QDialog
{
    Q_OBJECT

signals:
    void guestWindowB();//сигнал для возврата на форму гостя

public:
    explicit guestBrigades(QWidget *parent = nullptr);
    ~guestBrigades();

private slots:
    void on_btnBack_clicked();

    void on_btnFind_clicked(bool checked);
protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::guestBrigades *ui;
    QSqlDatabase dbconn;

public slots:
    void dbconnect();
    void selectAll();
};

#endif // GUESTBRIGADES_H
