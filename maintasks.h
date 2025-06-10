#ifndef MAINTASKS_H
#define MAINTASKS_H
#include <QListWidgetItem>
#include <QDialog>
#include <QTimer>
#include <QTime>
#include <QSoundEffect>
#include <QDateTime>
#include <algorithm>
#include <QBrush>

namespace Ui {
class MainTasks;
}

class MainTasks : public QDialog
{
    Q_OBJECT

public:
    explicit MainTasks(QWidget *parent = nullptr);
    ~MainTasks();

private slots:
    void on_startPomodoroButton_clicked();

    void updateDisplay();

    void setDisplay(int time);

    void setTimer(int time);

    void on_pomodoroButton_clicked();

    void on_shortBreakButton_clicked();

    void on_longBreakButton_clicked();

    void moveAddTaskButton();

    void on_addTaskButton_clicked();

    void on_cancelNewTaskButton_clicked();

    void on_updatePasswordButton_clicked();

    void on_removeAccountButton_clicked();

    void on_confirmTaskAddButton_clicked();

    void refreshTaskList();

    void updateProfileStats();

    void on_taskListDisplay_itemDoubleClicked(QListWidgetItem *item);

    void on_createTeamButton_clicked();

    void on_addMembersButton_clicked();

    void on_crateTeamCancelButton_clicked();

    void on_addMemberCancelButton_clicked();

    void on_leaveJoinTeamButton_clicked();

    void on_createTeamConfirmButton_clicked();

    void on_allTeamsComboBox_currentIndexChanged(int index);

    void on_addMemberConfimButton_clicked();

    void on_sortTasksComboBox_currentIndexChanged(int index);

private:
    Ui::MainTasks *ui;

    //Timer
    QTimer *pomodoroTimer;
    int remainingTime;
    int startingTime;
    bool isRunning = false;
    QSoundEffect *timerEndSound;

    //Add task button
    QPushButton *addTaskButton;

    void loadAllTeamsToComboBox();

    // Task Sorting
    enum TaskSortCriteria { // <<< Updated enum
        SortByDueDateAsc,
        SortByDueDateDesc,
        SortByNameAsc,
        SortByNameDesc
    };
    TaskSortCriteria currentTaskSortCriteria;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
};

#endif // MAINTASKS_H
