#include "maintasks.h"
#include "ui_maintasks.h"

MainTasks::MainTasks(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainTasks)
{
    ui->setupUi(this);

    //Tab names
    ui->tabWidget->setTabText(0, "Tasks");
    ui->tabWidget->setTabText(1, "Pomodoro");
    ui->tabWidget->setTabText(2, "Profile");

    //Profile
    ui->removeAccounButton->setStyleSheet("background-color: red; color: white;");
}

MainTasks::~MainTasks()
{
    delete ui;
}
