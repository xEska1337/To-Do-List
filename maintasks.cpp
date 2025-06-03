#include "maintasks.h"
#include "ui_maintasks.h"
#include "mainwindow.h"


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
    ui->removeAccountButton->setStyleSheet("background-color: red; color: white;");
    ui->profileUsername->setText(QString::fromStdString(MainWindow::currentUser.getUsername()));


    //Timer
    pomodoroTimer = new QTimer(this);
    connect(pomodoroTimer, &QTimer::timeout, this, &MainTasks::updateDisplay);
    ui->pomodoroTimerDisplay->setStyleSheet("background-color: #923737");
    remainingTime = 25 * 60;
    startingTime = remainingTime;
    setDisplay(remainingTime);
    ui->pomodoroProgressBar->setRange(0, 100);
    ui->pomodoroProgressBar->setValue(0);
    //Timer end sound
    timerEndSound = new QSoundEffect(this);
    timerEndSound->setSource(QUrl("qrc:/sounds/timerEnd.wav"));
    timerEndSound->setVolume(0.4);

    //Add task button
    ui->addTaskButton->setParent(ui->taskListDisplay);
    ui->addTaskButton->raise();
    ui->addTaskButton->setFixedSize(40,40);
}

MainTasks::~MainTasks()
{
    delete ui;
}

void MainTasks::on_startPomodoroButton_clicked()
{
    if(isRunning){
        pomodoroTimer->stop();
        isRunning = false;
        ui->startPomodoroButton->setText("Start");
    } else{
        if (remainingTime <= 0) {
            on_pomodoroButton_clicked();
        }
        pomodoroTimer->start(1000);
        isRunning = true;
        updateDisplay();
        ui->startPomodoroButton->setText("Stop");
    }

}

void MainTasks::updateDisplay()
{
    if(remainingTime > 0){
        remainingTime--;

        setDisplay(remainingTime);

        int progress = 100 * (startingTime - remainingTime) / startingTime;
        ui->pomodoroProgressBar->setValue(progress);

    } else {
        pomodoroTimer->stop();
        isRunning = false;
        setDisplay(0);
        timerEndSound->play();
        ui->startPomodoroButton->setText("Start");
    }
}

void MainTasks::setDisplay(int time)
{
    int min = time / 60;
    int sec = time % 60;
    QTime dispTime(0, min, sec);

    ui->pomodoroTimerDisplay->display(dispTime.toString("mm:ss"));
}

void MainTasks::setTimer(int time)
{
    remainingTime = time;
    startingTime = remainingTime;
    setDisplay(remainingTime);
    if(isRunning) {
        on_startPomodoroButton_clicked();
        ui->pomodoroProgressBar->setValue(0);
    }
}


void MainTasks::on_pomodoroButton_clicked()
{
    setTimer(25 * 60);
    ui->pomodoroTimerDisplay->setStyleSheet("background-color: #923737");
}


void MainTasks::on_shortBreakButton_clicked()
{
    setTimer(5 * 60);
    ui->pomodoroTimerDisplay->setStyleSheet("background-color: #2d6a6e");
}


void MainTasks::on_longBreakButton_clicked()
{
    setTimer(15 * 60);
    ui->pomodoroTimerDisplay->setStyleSheet("background-color: #2d6a6e");
}

void MainTasks::moveAddTaskButton(){
    //Pin button to right bottom corner
    int margin = 10;
    ui->addTaskButton->move(
        ui->taskListDisplay->width() - ui->addTaskButton->width() - margin,
        ui->taskListDisplay->height() - ui->addTaskButton->height() - margin
        );
}

void MainTasks::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    moveAddTaskButton();
}

void MainTasks::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    moveAddTaskButton();
}

void MainTasks::on_addTaskButton_clicked()
{
    //Switch view
    ui->stackedWidget->setCurrentIndex(1);

    //Set minimum task due date
    ui->taskDueDate->setDateTime(QDateTime::currentDateTime());
    ui->taskDueDate->setMinimumDateTime(QDateTime::currentDateTime());

    //Adding teams to list
    ui->teamSelect->clear();
    //ui->teamSelect->addItem("TeamName");
}

void MainTasks::on_cancelNewTaskButton_clicked() {
    //Switch view
    ui->stackedWidget->setCurrentIndex(0);
    moveAddTaskButton();

    //Clear task form
    ui->taskName->clear();
    ui->colabInput->clear();
    ui->taskDescription->clear();

}
void MainTasks::on_updatePasswordButton_clicked()
{
    QString currentPassword = ui->profilePassword->text();
    QString newPassword = ui->profilePasswordConfirm->text();

    if (currentPassword.isEmpty() || newPassword.isEmpty()) {
        QMessageBox::warning(this, "Error", "Both password fields are required!");
        return;
    }

    if (newPassword.length() < 4) {
        QMessageBox::warning(this, "Error", "New password must be at least 4 characters long!");
        return;
    }

    // Check if the current password is correct
    User currentUser = MainWindow::currentUser;
    User tempUser(currentUser.getUsername(), currentPassword.toStdString());
    if (tempUser.getPassword() != currentUser.getPassword()) {
        QMessageBox::warning(this, "Error", "Current password is incorrect!");
        return;
    }

    // Change the password in the database
    User newUser(currentUser.getId(), currentUser.getUsername(), newPassword.toStdString(), currentUser.getCreationDate());

    UserManager::updateUser(newUser);

    MainWindow::currentUser.setPassword(newPassword.toStdString());

    QMessageBox::information(this, "Success", "Password changed successfully!");
    ui->profilePassword->clear();
    ui->profilePasswordConfirm->clear();
}

void MainTasks::on_removeAccountButton_clicked()
{
    // User confirmation
    auto reply = QMessageBox::question(this, "Delete Account",
        "Are you sure you want to delete your account? This action cannot be undone.",
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes)
        return;

    User currentUser = MainWindow::currentUser;
    /*QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database connection failed!");
        return;
    }*/

    // Delete the user from the database
    UserManager::deleteUser(currentUser.getId());

    //  Delete the user's tasks
    /*QSqlQuery taskQuery(db);
    taskQuery.prepare("DELETE FROM tasks WHERE userAssigned = ?");
    taskQuery.addBindValue(currentUser.getId());
    taskQuery.exec();*/

    QMessageBox::information(this, "Account Deleted", "Your account has been deleted.");

    // Open the login window
    MainWindow *loginWindow = new MainWindow();
    loginWindow->show();

    this->close(); //Close the tasks window
}
