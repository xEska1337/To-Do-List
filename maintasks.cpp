#include "maintasks.h"
#include "ui_maintasks.h"
#include "mainwindow.h"
#include "taskmanager.h"
#include <QListWidgetItem>
#include <QSqlQuery>
#include <QSqlError>
#include "teammanager.h"
#include <QStringListModel>
#include <QInputDialog>

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
    loadAllTeamsToComboBox();

    // Task Sorting
    currentTaskSortCriteria = SortByDueDateAsc;
    ui->sortTasksComboBox->clear();
    ui->sortTasksComboBox->setStyleSheet("QComboBox#sortTasksComboBox { margin-left: 10px; margin-right: 10px; }");
    ui->sortTasksComboBox->addItem("Date (Oldest First)", SortByDueDateAsc);
    ui->sortTasksComboBox->addItem("Date (Newest First)", SortByDueDateDesc);
    ui->sortTasksComboBox->addItem("Name (A-Z)", SortByNameAsc);
    ui->sortTasksComboBox->addItem("Name (Z-A)", SortByNameDesc);
    ui->sortTasksComboBox->setCurrentIndex(ui->sortTasksComboBox->findData(currentTaskSortCriteria));

    if (!connect(ui->sortTasksComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                 this, &MainTasks::on_sortTasksComboBox_currentIndexChanged)) {}

}

MainTasks::~MainTasks()
{
    delete ui;
}

//
//Pomodoro
//

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

/**
 * @brief Updates Pomodoro Timer
 */
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

/**
 * @brief Changes Pomodoro Timer's display time
 */
void MainTasks::setDisplay(int time)
{
    int min = time / 60;
    int sec = time % 60;
    QTime dispTime(0, min, sec);

    ui->pomodoroTimerDisplay->display(dispTime.toString("mm:ss"));
}

/**
 * @brief Changes Pomodoro Timer's time
 */
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

//
//Tasks
//

/**
 * @brief Changes Task Button position in UI
 */
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

void MainTasks::on_addTaskButton_clicked() {
    //Switch view
    ui->stackedWidget->setCurrentIndex(1);

    //Set minimum task due date
    ui->taskDueDate->setDateTime(QDateTime::currentDateTime());
    ui->taskDueDate->setMinimumDateTime(QDateTime::currentDateTime());

    //Adding teams to list
    ui->teamSelect->clear();
    //ui->teamSelect->addItem("TeamName");
    ui->teamSelect->addItem("No team", QVariant(0));
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT id, name, members FROM teams");
    if (query.exec()) {
        while (query.next()) {
            uint32_t teamId = query.value("id").toUInt();
            QString teamName = query.value("name").toString();
            QString membersStr = query.value("members").toString();
            // Check if user is a member
            QStringList membersList = membersStr.split(';', Qt::SkipEmptyParts);
            for (const QString& member : membersList) {
                if (member.toUInt() == MainWindow::currentUser.getId()) {
                    ui->teamSelect->addItem(teamName + " (Team task)", QVariant(teamId));  // Dodano "(Team task)"
                    break;
                }
            }
        }
    }
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

    newTask.setDeadline(dueDate.toSecsSinceEpoch());

    uint32_t selectedTeamId = ui->teamSelect->currentData().toUInt();
    if (selectedTeamId == 0) {
        // Personal task
        newTask.setUserId(MainWindow::currentUser.getId());
        newTask.setTeamId(0);
    } else {
        // Team task
        newTask.setUserId(0);  // 0 means it's a team task
        newTask.setTeamId(selectedTeamId);
    }
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

/**
 * @brief Updates Task list in UI
 */
void MainTasks::refreshTaskList()
{
    // Clear the current task list display
    ui->taskListDisplay->clear();
    uint32_t userId = MainWindow::currentUser.getId();

    // Get all tasks for the current user
    auto userTasks = TaskManager::getTasksForUser(userId);

    // Get tasks for user teams
    std::vector<Team> userTeams = TeamManager::getTeamsForUser(userId);
    std::vector<Task> teamTasks;

    for (const auto& team : userTeams) {
        auto tasks = TaskManager::getTasksForTeam(team.getId());
        teamTasks.insert(teamTasks.end(), tasks.begin(), tasks.end());
    }


    //Combine tasks
    std::vector<Task> allTasks = userTasks;
    allTasks.insert(allTasks.end(), teamTasks.begin(), teamTasks.end());

    //Sorting
    std::sort(allTasks.begin(), allTasks.end(), [this](const Task& a, const Task& b) {
        bool a_has_deadline = a.getDeadline() != 0;
        bool b_has_deadline = b.getDeadline() != 0;

        switch (currentTaskSortCriteria) {
            case SortByNameAsc:
                return QString::fromStdString(a.getName()).toLower() < QString::fromStdString(b.getName()).toLower();
            case SortByNameDesc:
                return QString::fromStdString(a.getName()).toLower() > QString::fromStdString(b.getName()).toLower();
            case SortByDueDateAsc:
                if (a_has_deadline && !b_has_deadline) return true;
                if (!a_has_deadline && b_has_deadline) return false;
                if (!a_has_deadline && !b_has_deadline) {
                    return QString::fromStdString(a.getName()).toLower() < QString::fromStdString(b.getName()).toLower();
                }
                return a.getDeadline() < b.getDeadline();
            case SortByDueDateDesc:
                if (a_has_deadline && !b_has_deadline) return true;
                if (!a_has_deadline && b_has_deadline) return false;
                if (!a_has_deadline && !b_has_deadline) {
                    return QString::fromStdString(a.getName()).toLower() < QString::fromStdString(b.getName()).toLower();
                }
                return a.getDeadline() > b.getDeadline();
        }
        return false;
    });

    qint64 currentTime = QDateTime::currentDateTime().toSecsSinceEpoch();

    for (const auto& task : allTasks) {
        QString display = QString::fromStdString(task.getName());
        if (!task.getDescription().empty()) {
            display += " - " + QString::fromStdString(task.getDescription());
        }

        if (task.getDeadline() > 0) {
            QDateTime due = QDateTime::fromSecsSinceEpoch(task.getDeadline());
            display += " (Due: " + due.toString("yyyy-MM-dd HH:mm") + ")";
        }

        if (task.getUserId() == 0 && task.getTeamId() != 0) {
            Team team = TeamManager::getTeam(task.getTeamId());
            if (team.getId() != 0) {
                QString teamName = QString::fromStdString(team.getName());
                display = "[" + teamName + "] " + display;
            } else {
                display = "[Unknown Team] " + display;
            }
        }

        auto *item = new QListWidgetItem(display);
        item->setData(Qt::UserRole, static_cast<qulonglong>(task.getId()));

        // Change color if past due date
        if (task.getDeadline() > 0 && task.getDeadline() < currentTime) {
            item->setForeground(Qt::red);
        }
        ui->taskListDisplay->addItem(item);
    }

    // Update the profile statistics after refreshing the task list
    updateProfileStats();
    moveAddTaskButton();
}

void MainTasks::on_sortTasksComboBox_currentIndexChanged(int index)
{
    if (index < 0) return;
    currentTaskSortCriteria = static_cast<TaskSortCriteria>(ui->sortTasksComboBox->itemData(index).toInt());
    refreshTaskList();
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

//
//Profile
//

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

/**
 * @brief Updates logged in User's profile stats
 */
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

//
//Teams
//

void MainTasks::on_createTeamButton_clicked() {
    ui->stackedWidget_2->setCurrentIndex(1);
    //Populate user list
    ui->crateTeamMemberAddList->clear();
    std::vector<User> allUsers = UserManager::getAllUsers();

    for (const auto& user : allUsers) {
        // Skip the current user, as they are the creator and added by default
        if (user.getId() == MainWindow::currentUser.getId()) {
            continue;
        }

        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(user.getUsername()), ui->crateTeamMemberAddList);
        item->setData(Qt::UserRole, QVariant::fromValue(user.getId()));
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

void MainTasks::on_crateTeamCancelButton_clicked() {
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->createTeamTeamName->clear();
    ui->createTeamNameConfirm->clear();
    ui->crateTeamMemberAddList->clear();
    ui->createTeamPassword->clear();
    ui->createTeamPasswordConfirm->clear();
}

void MainTasks::on_addMembersButton_clicked() {
   int comboBoxIndex = ui->allTeamsComboBox->currentIndex();
    if (comboBoxIndex < 0) {
        QMessageBox::warning(this, "No Team Selected", "Please select a team from the list first.");
        return;
    }

    Team selectedTeam = TeamManager::getTeam(ui->allTeamsComboBox->currentData().toUInt());

    if (selectedTeam.getId() == 0) {
        QMessageBox::critical(this, "Error", "Could not retrieve team details.");
        return;
    }

    // Check if the current user is a member of the team
    if (!selectedTeam.containsUser(MainWindow::currentUser.getId())) {
        QMessageBox::warning(this, "Access Denied", "You must be a member of this team to add other users.");
        return;
    }

    ui->stackedWidget_2->setCurrentIndex(2);

    // Populate list with users not already in the team
    ui->addMemberList->clear();
    std::vector<User> allUsers = UserManager::getAllUsers();
    std::vector<uint32_t> currentTeamMembers = selectedTeam.getMembers();

    for (const auto& user : allUsers) {
        if (user.getId() == MainWindow::currentUser.getId()) {
            continue;
        }

        bool isAlreadyMember = false;
        for (uint32_t memberId : currentTeamMembers) {
            if (user.getId() == memberId) {
                isAlreadyMember = true;
                break;
            }
        }
        if (isAlreadyMember) {
            continue;
        }

        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(user.getUsername()), ui->addMemberList);
        item->setData(Qt::UserRole, QVariant::fromValue(user.getId()));
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

void MainTasks::on_addMemberCancelButton_clicked() {
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->addMemberList->clear();
}

void MainTasks::on_addMemberConfimButton_clicked() {
    uint32_t teamId = ui->allTeamsComboBox->currentData().toUInt();
    if (teamId == 0) {
        QMessageBox::critical(this, "Error", "No team context for adding members. Please try again.");
        on_addMemberCancelButton_clicked();
        return;
    }

    Team teamToUpdate = TeamManager::getTeam(teamId);
    if (teamToUpdate.getId() == 0) {
        QMessageBox::critical(this, "Error", "Could not retrieve team details for update.");
        on_addMemberCancelButton_clicked();
        return;
    }

    std::vector<uint32_t> membersToAdd;
    for (int i = 0; i < ui->addMemberList->count(); ++i) {
        QListWidgetItem *item = ui->addMemberList->item(i);
        if (item->checkState() == Qt::Checked) {
            membersToAdd.push_back(item->data(Qt::UserRole).toUInt());
        }
    }

    if (membersToAdd.empty()) {
        QMessageBox::information(this, "No Selection", "No new members were selected to add.");
        return;
    }

    for (uint32_t userId : membersToAdd) {
        teamToUpdate.addMember(userId);
    }

    if (TeamManager::updateTeam(teamToUpdate)) {
        QMessageBox::information(this, "Success", "Selected members have been added to the team.");
        loadAllTeamsToComboBox();

        if (ui->allTeamsComboBox->currentData().isValid() && ui->allTeamsComboBox->currentData().toUInt() == teamId) {
            on_allTeamsComboBox_currentIndexChanged(ui->allTeamsComboBox->currentIndex());
        }
    } else {
        QMessageBox::critical(this, "Error", "Failed to add members to the team.");
    }

    on_addMemberCancelButton_clicked();
}

void MainTasks::on_leaveJoinTeamButton_clicked() {

    int index = ui->allTeamsComboBox->currentIndex();
    if (index < 0) return;
    uint32_t teamId = ui->allTeamsComboBox->currentData().toUInt();
    Team team = TeamManager::getTeam(teamId);

    uint32_t userId = MainWindow::currentUser.getId();

    if (team.containsUser(userId)) {
        // User is a member - logic "Leave team"
        if (QMessageBox::question(this, "Leave team", "Are you sure you want to leave this team?") != QMessageBox::Yes)
            return;

        team.removeMember(userId);

        if (team.getMembers().empty()) {
            TeamManager::deleteTeam(teamId);
            QMessageBox::information(this, "Info", "The group has been removed because it no longer has any members..");
            loadAllTeamsToComboBox();
        } else {
            TeamManager::updateTeam(team);
            QMessageBox::information(this, "Info", "You left the team.");
        }
    } else {
        // User is not a member - logic "Join team"
        bool ok;
        QString password = QInputDialog::getText(this, "Join team", "Enter your team password:", QLineEdit::Password, "", &ok).trimmed();
        if (!ok || password.isEmpty()) return;
        // check password
        Team tempTeam(team.getName(), password.toStdString(), {});
        if (tempTeam.getPassword() != team.getPassword()) {
            QMessageBox::warning(this, "Error", "Incorrect password!");
            return;
        }

        // Add user to team
        team.addMember(userId);
        TeamManager::updateTeam(team);
        QMessageBox::information(this, "Success", "Joined the team!");
    }

    //Refresh
    if (ui->allTeamsComboBox->currentData().isValid() && ui->allTeamsComboBox->currentData().toUInt() == teamId) {
        on_allTeamsComboBox_currentIndexChanged(ui->allTeamsComboBox->currentIndex());
    }
}

void MainTasks::on_createTeamConfirmButton_clicked()
{
    QString teamName = ui->createTeamTeamName->text().trimmed();
    QString teamNameConfirm = ui->createTeamNameConfirm->text().trimmed();
    QString teamPassword = ui->createTeamPassword->text().trimmed();
    QString teamPasswordConfirm = ui->createTeamPasswordConfirm->text().trimmed();


    if (teamName.isEmpty() || teamNameConfirm.isEmpty()) {
        QMessageBox::warning(this, "Error", "Team name cannot be empty!");
        return;
    }

    if (teamName != teamNameConfirm) {
        QMessageBox::warning(this, "Error", "Team names do not match!");
        return;
    }

    if (teamPassword.isEmpty() || teamPasswordConfirm.isEmpty()) {
        QMessageBox::warning(this, "Error", "Team password cannot be empty!");
        return;
    }

    if (teamPassword != teamPasswordConfirm) {
        QMessageBox::warning(this, "Error", "Team passwords do not match!");
        return;
    }

    if (teamPassword.length() < 4) {
        QMessageBox::warning(this, "Error", "Team password must be at least 4 characters long!");
        return;
    }

    // Creating a team with a current user as the first member
    std::vector<uint32_t> members;
    members.push_back(MainWindow::currentUser.getId());

    // Add selected users from the list
    for (int i = 0; i < ui->crateTeamMemberAddList->count(); ++i) {
        QListWidgetItem *item = ui->crateTeamMemberAddList->item(i);
        if (item->checkState() == Qt::Checked) {
            uint32_t selectedUserId = item->data(Qt::UserRole).toUInt();
            if (selectedUserId != MainWindow::currentUser.getId()) {
                members.push_back(selectedUserId);
            }
        }
    }

    Team newTeam(teamName.toStdString(), teamPassword.toStdString(), members);

    // Zapisanie zespołu do bazy danych
    if (TeamManager::createTeam(newTeam)) {
        QMessageBox::information(this, "Success", "Team created successfully!");

        // Wyczyszczenie formularza
        ui->createTeamTeamName->clear();
        ui->createTeamNameConfirm->clear();
        ui->createTeamPassword->clear();
        ui->createTeamPasswordConfirm->clear();
        ui->crateTeamMemberAddList->clear();

        //Back to main teams view
        ui->stackedWidget_2->setCurrentIndex(0);
        loadAllTeamsToComboBox();

    } else {
        QMessageBox::critical(this, "Error", "Failed to create team!");
    }
}

void MainTasks::loadAllTeamsToComboBox() {
    ui->allTeamsComboBox->clear();
    auto allTeams = TeamManager::getAllTeams();
    for (const auto& team : allTeams) {
        ui->allTeamsComboBox->addItem(QString::fromStdString(team.getName()), QVariant::fromValue(team.getId()));
    }
    if (ui->allTeamsComboBox->count() > 0)
        ui->allTeamsComboBox->setCurrentIndex(0);
}
void MainTasks::on_allTeamsComboBox_currentIndexChanged(int index) {
    if (index < 0) {
        // No team selected, or combobox is empty
        ui->teamMembersDisplay->setModel(nullptr);
        ui->leaveJoinTeamButton->setText("Join/Leave Team");
        ui->leaveJoinTeamButton->setEnabled(false);
        ui->addMembersButton->setEnabled(false);
        return;
    }
    uint32_t teamId = ui->allTeamsComboBox->currentData().toUInt();
    Team selectedTeam = TeamManager::getTeam(teamId);

    ui->leaveJoinTeamButton->setEnabled(true);
    ui->addMembersButton->setEnabled(true);
    ui->teamMembersDisplay->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Update button text based on team
    if (selectedTeam.containsUser(MainWindow::currentUser.getId())) {
        ui->leaveJoinTeamButton->setText("Leave Team");
    } else {
        ui->leaveJoinTeamButton->setText("Join Team");
    }

    // Populate team members display (existing logic)
    QStringListModel *model = new QStringListModel(this); // Consider making this a member to avoid leaks if not parented well
    QStringList memberNames;
    std::vector<User> members = selectedTeam.getMembersAsUsers();
    for (const auto& member : members) {
        memberNames << QString::fromStdString(member.getUsername());
    }
    model->setStringList(memberNames);
    ui->teamMembersDisplay->setModel(model);
}