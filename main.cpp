#include "mainwindow.h"

#include <QApplication>
#include <QIcon>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "team.h"
#include "teammanager.h"
#include "usermanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    //Create database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("todolist.db");

    if (!db.open()) {
        qFatal("Failed to open database: %s", qUtf8Printable(db.lastError().text()));
    }

    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS users ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "username TEXT NOT NULL, "
                        "password BIGINT UNSIGNED NOT NULL, "
                        "completedTasks INTEGER, "
                        "creationDate DATE)");

    query.exec("CREATE TABLE IF NOT EXISTS tasks ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "userAssigned INTEGER NOT NULL, "
                        "collaborators TEXT, "
                        "name TEXT NOT NULL, "
                        "dueDate DATETIME, "
                        "description TEXT, "
                        "FOREIGN KEY(userAssigned) REFERENCES users(id))");

    query.exec("CREATE TABLE IF NOT EXISTS teams ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "name TEXT NOT NULL, "
                        "password BIGINT UNSIGNED NOT NULL, "
                        "members LONGTEXT NOT NULL)");

    //Set icon
    a.setWindowIcon(QIcon(":/icons/main.ico"));

    //Set stylesheet
    QFile styleSheetFile(":/Combinear.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    a.setStyleSheet(styleSheet);

    MainWindow w;
    w.show();
    return a.exec();
}
