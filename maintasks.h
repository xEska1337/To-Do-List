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

private:
    Ui::MainTasks *ui;

    //Timer
    QTimer *pomodoroTimer;
    int remainingTime;
    int startingTime;
    bool isRunning = false;
    QSoundEffect *timerEndSound;
};

#endif // MAINTASKS_H
