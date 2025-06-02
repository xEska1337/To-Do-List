#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include "maintasks.h"
#include "createuser.h"
#include "user.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static User currentUser; // Static member to store current logged user

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();

private:
    Ui::MainWindow *ui;
    MainTasks *taskWindow;
    CreateUser *createUserWindow;

    bool authenticateUser(const QString& username, const QString& password);
    User getCurrentUser() const;


};

#endif // MAINWINDOW_H