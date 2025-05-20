#ifndef CREATEUSER_H
#define CREATEUSER_H

#include <QDialog>

namespace Ui {
class CreateUser;
}

class CreateUser : public QDialog
{
    Q_OBJECT

public:
    explicit CreateUser(QWidget *parent = nullptr);
    ~CreateUser();

private slots:
    void on_createNewAccountButton_clicked();

private:
    Ui::CreateUser *ui;
};

#endif // CREATEUSER_H
