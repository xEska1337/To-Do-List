#ifndef JOINTEAM_H
#define JOINTEAM_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
    class JoinTeam;
}
QT_END_NAMESPACE

class JoinTeam : public QDialog
{
    Q_OBJECT

public:
    JoinTeam(QWidget *parent = nullptr);
    ~JoinTeam();
private:
    Ui::JoinTeam *ui;
};

#endif // JOINTEAM_H
