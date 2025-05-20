#include "createuser.h"
#include "ui_createuser.h"

CreateUser::CreateUser(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateUser)
{
    ui->setupUi(this);
}

CreateUser::~CreateUser()
{
    delete ui;
}

void CreateUser::on_createNewAccountButton_clicked()
{
    this->close();
}

