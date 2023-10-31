#ifndef SOMESELECT_H
#define SOMESELECT_H

#include <QDialog>

namespace Ui {
class someselect;
}

class someselect : public QDialog
{
    Q_OBJECT

public:
    explicit someselect(QWidget *parent = nullptr);
    ~someselect();

private:
    Ui::someselect *ui;
};

#endif // SOMESELECT_H
