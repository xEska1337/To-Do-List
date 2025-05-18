#include "maintasks.h"
#include "ui_maintasks.h"

MainTasks::MainTasks(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainTasks)
{
    ui->setupUi(this);
    ui->tabWidget->setTabText(0, "Tasks");
    ui->tabWidget->setTabText(1, "Pomodoro");
    ui->tabWidget->setTabText(2, "Profile");
}

MainTasks::~MainTasks()
{
    delete ui;
}
