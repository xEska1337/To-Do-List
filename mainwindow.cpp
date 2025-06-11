#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "maintasks.h"
#include <QPixmap>
#include "createuser.h"

User MainWindow::currentUser; // Definition of static member

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
    QString username = ui->loginForm->text().trimmed();
    QString password = ui->passwordForm->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter both username and password!");
        return;
    }

    if (authenticateUser(username, password)) {
        taskWindow = new MainTasks();
        taskWindow->show();
        this->close();
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password!");
        ui->passwordForm->clear();
    }
}

void MainWindow::on_registerButton_clicked()
{
    createUserWindow = new CreateUser(this);
    createUserWindow->exec(); // Use exec() for modal dialog
}

/**
 * Authenticates user with specified name and password
 * @param username User's name
 * @param password Password to authenticate
 * @return True if success, false if not
 */
bool MainWindow::authenticateUser(const QString& username, const QString& password)
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database connection failed!");
        return false;
    }

    // Hash the input password using the same method as User class
    User tempUser(username.toStdString(), password.toStdString());
    std::string hashedPassword = tempUser.getPassword();

    QSqlQuery query(db);
    query.prepare("SELECT id, username, password FROM users WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(QString::fromStdString(hashedPassword));

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error",
                             "Login query failed: " + query.lastError().text());
        return false;
    }

    if (query.next()) {
        // User found, store current user info
        uint32_t userId = query.value(0).toUInt();
        std::string dbUsername = query.value(1).toString().toStdString();
        std::string dbPassword = query.value(2).toString().toStdString();

        currentUser = User(userId, dbUsername, dbPassword);
        return true;
    }

    return false;
}

/**
 * Returns User currently logged in
 * @return Current logged in User
 */
User MainWindow::getCurrentUser() const
{
    return currentUser;
}