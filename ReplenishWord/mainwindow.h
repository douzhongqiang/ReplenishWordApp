#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QToolButton>
#include "ReplenishWordMainWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    ReplenishWordMainWidget* m_pMainWidget = nullptr;
    QToolBar* m_pToolBar = nullptr;

    void initToolBar(void);

private slots:
    void onMoveButtonCheckedStatusChanged(void);
    void onResizeButtonCheckedStatusChanged(void);
    void onRotateButtonCheckedStatusChanged(void);

    void onHandleButtonCheckedStatusChanged(void);
    void onDeleteButtonCheckedStatusChanged(void);
    void onSpitButtonCheckedStatusChanged(void);
};

#endif // MAINWINDOW_H
