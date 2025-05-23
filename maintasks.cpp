#include "maintasks.h"
#include "ui_maintasks.h"

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

