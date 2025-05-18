#include "mainwindow.h"

#include <QApplication>
#include <QIcon>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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
