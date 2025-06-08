#include "maintasks.h"
#include "ui_maintasks.h"
#include "mainwindow.h"
#include "taskmanager.h"
#include <QListWidgetItem>
#include <QSqlQuery>
#include <QSqlError>

MainTasks::MainTasks(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainTasks)
{
    ui->setupUi(this);

    ui->taskDueDate->setMinimumDateTime(QDateTime::currentDateTime());

    //Tab names
    ui->tabWidget->setTabText(0, "Tasks");
    ui->tabWidget->setTabText(1, "Pomodoro");
    ui->tabWidget->setTabText(2, "Profile");
    ui->tabWidget->setTabText(3, "Teams");

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

    ui->profileUsername->setText(QString::fromStdString(MainWindow::currentUser.getUsername()));
    updateProfileStats();
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
    refreshTaskList();
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
    ui->taskDescription->clear();
    ui->teamSelect->clear();

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

    // Delete the user from the database
    UserManager::deleteUser(currentUser.getId());

    QMessageBox::information(this, "Account Deleted", "Your account has been deleted.");

    // Open the login window
    MainWindow *loginWindow = new MainWindow();
    loginWindow->show();

    this->close(); //Close the tasks window
}
void MainTasks::on_confirmTaskAddButton_clicked()
{
    QString taskName = ui->taskName->text().trimmed();
    QString taskDescription = ui->taskDescription->toPlainText().trimmed();
    QDateTime dueDate = ui->taskDueDate->dateTime();

    if (taskName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Task name cannot be empty!");
        return;
    }

    Task newTask;
    newTask.setName(taskName.toStdString());
    newTask.setDescription(taskDescription.toStdString());
    newTask.setUserId(MainWindow::currentUser.getId());
    newTask.setDeadline(dueDate.toSecsSinceEpoch());
    QSqlDatabase db = QSqlDatabase::database();
    if (db.isOpen()) {
        QSqlQuery query(db);
        query.prepare("UPDATE users SET uncompletedTasks = COALESCE(uncompletedTasks, 0) + 1 WHERE id = ?");
        query.addBindValue(MainWindow::currentUser.getId());
        query.exec();
    }

    if (TaskManager::createTask(newTask)) {
        QMessageBox::information(this, "Success", "Task created successfully!");
        ui->taskName->clear();
        ui->taskDescription->clear();
        ui->taskDueDate->setDateTime(QDateTime::currentDateTime());
        ui->stackedWidget->setCurrentIndex(0);
        refreshTaskList();
    } else {
        QMessageBox::critical(this, "Error", "Failed to create task!");
    }
}

void MainTasks::refreshTaskList()
{
    // Clear the current task list display
    ui->taskListDisplay->clear();
    uint32_t userId = MainWindow::currentUser.getId();
    // Get all tasks for the current user
    auto tasks = TaskManager::getTasksForUser(userId);
    for (const auto& task : tasks) {
        QString display = QString::fromStdString(task.getName());
        if (!task.getDescription().empty()) {
            display += " - " + QString::fromStdString(task.getDescription());
        }
        QDateTime due = QDateTime::fromSecsSinceEpoch(task.getDeadline());
        display += " (Due: " + due.toString("yyyy-MM-dd HH:mm") + ")";

        // Store the task ID in the QListWidgetItem for later reference
        auto *item = new QListWidgetItem(display);
        item->setData(Qt::UserRole, static_cast<qulonglong>(task.getId()));
        ui->taskListDisplay->addItem(item);
    }

    // Update the profile statistics after refreshing the task list
    updateProfileStats();
    moveAddTaskButton();
}

void MainTasks::updateProfileStats()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        // Show error if the database is not open
        ui->completedTaskStat->setText("Database Error");
        ui->uncompletedTaskStat->setText("");
        ui->accoungAgeStat->setText("");
        ui->completionPercStat->setText("");
        return;
    }

    QSqlQuery query(db);
    // Get completedTasks, uncompletedTasks, and account creation date for the current user
    query.prepare("SELECT completedTasks, uncompletedTasks, creationDate FROM users WHERE id = ?");
    query.addBindValue(MainWindow::currentUser.getId());

    if (!query.exec() || !query.next()) {
        // Show error if the query fails or returns no results
        ui->completedTaskStat->setText("Error loading stats");
        ui->uncompletedTaskStat->setText("");
        ui->accoungAgeStat->setText("");
        ui->completionPercStat->setText("");
        return;
    }

    // Use column indices for compatibility with all Qt versions
    int completedTasks = query.value(0).toInt();
    int uncompletedTasks = query.value(1).toInt();
    QDate creationDate = query.value(2).toDate();

    int accountAge = creationDate.daysTo(QDate::currentDate());
    int totalTasks = completedTasks + uncompletedTasks;
    double completionPercentage = totalTasks > 0 ? (double)completedTasks / totalTasks * 100 : 0;

    // Update the profile statistics labels
    ui->completedTaskStat->setText(QString("Completed tasks: %1").arg(completedTasks));
    ui->uncompletedTaskStat->setText(QString("Uncompleted tasks: %1").arg(uncompletedTasks));
    ui->accoungAgeStat->setText(QString("Account age: %1 days").arg(accountAge));
    ui->completionPercStat->setText(QString("Completion: %1%").arg(QString::number(completionPercentage, 'f', 1)));
}

void MainTasks::on_taskListDisplay_itemDoubleClicked(QListWidgetItem *item)
{
    if (!item) return;

    // Get the task ID stored in the QListWidgetItem
    qulonglong taskId = item->data(Qt::UserRole).toULongLong();

    // Ask the user for confirmation before marking the task as completed (deleting it)
    if (QMessageBox::question(this, "Complete Task",
                              "Mark this task as completed? It will be removed.",
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;

    // Create a temporary Task object with only the ID set (needed for deleteTask)
    Task tempTask;
    tempTask.setId(taskId);

    // Delete the task from the database
    if (!TaskManager::deleteTask(tempTask)) {
        QMessageBox::critical(this, "Error", "Failed to delete task!");
        return;
    }

    // Increment completedTasks and decrement uncompletedTasks (but not below 0)
    QSqlDatabase db = QSqlDatabase::database();
    if (db.isOpen()) {
        QSqlQuery query(db);
        query.prepare("UPDATE users SET "
                      "completedTasks = MAX(COALESCE(completedTasks, 0) + 1, 0), "
                      "uncompletedTasks = MAX(COALESCE(uncompletedTasks, 0) - 1, 0) "
                      "WHERE id = ?");
        query.addBindValue(MainWindow::currentUser.getId());
        if (!query.exec()) {
            QMessageBox::warning(this, "Warning",
                                 "Task removed but failed to update user stats: "
                                 + query.lastError().text());
        }
    }

    // Refresh the task list and profile statistics after deleting the task
    refreshTaskList();
}

void MainTasks::on_createTeamButton_clicked() {
    ui->stackedWidget_2->setCurrentIndex(1);
}

void MainTasks::on_crateTeamCancelButton_clicked() {
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->createTeamTeamName->clear();
    ui->createTeamNameConfirm->clear();
    ui->crateTeamMemberAddList->clear();
}

void MainTasks::on_addMembersButton_clicked() {
    ui->stackedWidget_2->setCurrentIndex(2);
}

void MainTasks::on_addMemberCancelButton_clicked() {
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->addMemberList->clear();
}

void MainTasks::on_leaveJoinTeamButton_clicked() {
    JoinTeamWindow = new JoinTeam(this);
    JoinTeamWindow->exec();
}