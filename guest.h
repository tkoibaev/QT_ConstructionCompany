#ifndef GUEST_H
#define GUEST_H

#include <QDialog>
#include<someselect.h>
#include<guestallobjects.h>
#include<guestmanagers.h>
#include<guestbrigades.h>
#include<guestapplications.h>

namespace Ui {
class guest;
}

class guest : public QDialog
{
    Q_OBJECT

public:
    explicit guest(QWidget *parent = nullptr);
    ~guest();

private slots:
    void on_btnBack_clicked();
    void on_btnOurObjects_clicked();
    void on_btnManagers_clicked();

    void on_btnBrigades_clicked();

    void on_btnApplication_clicked();

signals:
    void firstWindow();//сигнал для возврата на форму регистрации

private:
    Ui::guest *ui;
    guestAllObjects *allObjects;
    guestManagers *managers;
    guestBrigades *brigade;
    guestApplications *application;
};



#endif // GUEST_H
