#include "createuser.h"
#include "ui_createuser.h"

CreateUser::CreateUser(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateUser)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
}

CreateUser::~CreateUser()
{
    delete ui;
}

void CreateUser::on_createNewAccountButton_clicked()
{
    if (!validateInput()) {
        return;
    }

    QString username = ui->newUsername->text().trimmed();
    QString password = ui->newPassword->text();

    // Create User object
    User newUser(username.toStdString(), password.toStdString());

    // Replaced createUserInDatabase with UserManager::createUser
    if (UserManager::createUser(newUser)) {
        QMessageBox::information(this, "Success", "Account created successfully!");
        this->accept(); // Close dialog with success
    }
}

bool CreateUser::validateInput()
{
    QString username = ui->newUsername->text().trimmed();
    QString password = ui->newPassword->text();
    QString confirmPassword = ui->newPasswordConfirm->text();

    // Check if fields are empty
    if (username.isEmpty()) {
        QMessageBox::warning(this, "Error", "Username cannot be empty!");
        return false;
    }

    if (password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Password cannot be empty!");
        return false;
    }

    // Check password confirmation
    if (password != confirmPassword) {
        QMessageBox::warning(this, "Error", "Passwords do not match!");
        return false;
    }

    // Check minimum password length
    if (password.length() < 4) {
        QMessageBox::warning(this, "Error", "Password must be at least 4 characters long!");
        return false;
    }

    return true;
}

// Deprecated
// Consider using UserManager::createUser()
bool CreateUser::createUserInDatabase(const User& user)
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database connection failed!");
        return false;
    }

    // Check if username already exists
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM users WHERE username = ?");
    checkQuery.addBindValue(QString::fromStdString(user.getUsername()));

    if (!checkQuery.exec()) {
        QMessageBox::critical(this, "Database Error",
                             "Failed to check username: " + checkQuery.lastError().text());
        return false;
    }

    checkQuery.next();
    if (checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Error", "Username already exists!");
        return false;
    }

    // Insert new user
    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO users (username, password, creationDate) VALUES (?, ?, ?)");
    insertQuery.addBindValue(QString::fromStdString(user.getUsername()));
    insertQuery.addBindValue(QString::number(user.getPassword()));
    insertQuery.addBindValue(QDateTime::currentDateTime());

    if (!insertQuery.exec()) {
        QMessageBox::critical(this, "Database Error",
                             "Failed to create user: " + insertQuery.lastError().text());
        return false;
    }

    return true;
}