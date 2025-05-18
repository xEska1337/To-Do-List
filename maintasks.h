#ifndef MAINTASKS_H
#define MAINTASKS_H

#include <QDialog>

namespace Ui {
class MainTasks;
}

class MainTasks : public QDialog
{
    Q_OBJECT

public:
    explicit MainTasks(QWidget *parent = nullptr);
    ~MainTasks();

private:
    Ui::MainTasks *ui;
};

#endif // MAINTASKS_H
