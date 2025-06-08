#include "jointeam.h"
#include "ui_jointeam.h"

JoinTeam::JoinTeam(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::JoinTeam)
{
    ui->setupUi(this);
}

JoinTeam::~JoinTeam()
{
    delete ui;
}