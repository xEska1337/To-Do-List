#ifndef CREATEUSER_H
#define CREATEUSER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>
#include "user.h"
#include "usermanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class CreateUser;
}
QT_END_NAMESPACE

class CreateUser : public QDialog
{
    Q_OBJECT

public:
    CreateUser(QWidget *parent = nullptr);
    ~CreateUser();

private slots:
    void on_createNewAccountButton_clicked();

private:
    Ui::CreateUser *ui;
    bool validateInput();
    bool createUserInDatabase(const User& user);
};

#endif // CREATEUSER_H
