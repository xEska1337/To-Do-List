#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "maintasks.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    ui->imageLabel->setMinimumSize(150,150);
    QPixmap pixmap(":/icons/main.png");
    QPixmap scaledPixmap = pixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->imageLabel->setPixmap(scaledPixmap);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    taskWindow = new MainTasks();
    taskWindow->show();
    this->close();
}

