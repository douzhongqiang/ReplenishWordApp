#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_pMainWidget = new ReplenishWordMainWidget;
    this->setCentralWidget(m_pMainWidget);
}

MainWindow::~MainWindow()
{
}

