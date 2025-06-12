#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

    // Event handlers

    // Click handlers

    void on_loginButton_clicked();
    void on_registerButton_clicked();

private:
    Ui::MainWindow *ui;
    MainTasks *taskWindow;
    CreateUser *createUserWindow;

    /**
     * @brief Authenticates user with specified name and password
     *
     * @param username User's name
     * @param password Password to authenticate
     * @return True if success, false if not
     */
    bool authenticateUser(const QString& username, const QString& password);

    /**
     * @brief Returns User currently logged in
     *
     * @return Current logged in User
     */
    User getCurrentUser() const;


};

#endif // MAINWINDOW_H