#ifndef MAINTASKS_H
#define MAINTASKS_H

#include <QDialog>
#include <QTimer>
#include <QTime>
#include <QLCDNumber>
#include <QSoundEffect>


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

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
};

#endif // MAINTASKS_H
