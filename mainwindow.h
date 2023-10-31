#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <QString>
#include <QMainWindow>
#include <QMainWindow>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlDatabase>
#include <employee.h>
#include <guest.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnLog_clicked();

private:
    Ui::MainWindow *ui;
    guest *guest_window;
    employee *emp_window;

    //MainWindow *main_window;
};
#endif // MAINWINDOW_H
