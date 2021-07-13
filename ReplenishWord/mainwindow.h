#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ReplenishWordMainWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    ReplenishWordMainWidget* m_pMainWidget = nullptr;
};
#endif // MAINWINDOW_H
