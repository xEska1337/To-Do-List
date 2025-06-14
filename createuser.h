#ifndef CREATEUSER_H
#define CREATEUSER_H

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

    // Event handlers

    // Click handlers

    void on_createNewAccountButton_clicked();

private:
    Ui::CreateUser *ui;
    /**
     * @brief Validates if the input is correct.
     * @return True if the input is correct, false if not.
     */
    bool validateInput();

    // Deprecated
    // Consider using UserManager::createUser()
    bool createUserInDatabase(const User& user);
};

#endif // CREATEUSER_H
