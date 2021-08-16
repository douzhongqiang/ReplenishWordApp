#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/qss/ReplenishWord.qss");
    bool result = file.open(QFile::ReadOnly);
    if (result)
    {
        QString styleSheetString = file.readAll();
        a.setStyleSheet(styleSheetString);
    }

    MainWindow w;
    w.resize(800, 600);
    w.show();
    return a.exec();
}
