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
    void initConnect(void);

private slots:
    void onMoveButtonCheckedStatusChanged(void);

    void onHandleButtonCheckedStatusChanged(void);
    void onDeleteButtonCheckedStatusChanged(void);
    void onSpitButtonCheckedStatusChanged(void);
    void onAdsorbButtonChecledStatusChanged(void);

private:
    QToolButton* m_pSelectedButton = nullptr;
    QToolButton* m_pHandleButton = nullptr;
    QToolButton* m_pHandleDeleteButton = nullptr;
    QToolButton* m_pHandleSplitButton = nullptr;
    QToolButton* m_pHandleAdsorbButton = nullptr;

private slots:
    void onHandleEnabledChanged(bool isEnabled);
    void onHandlePointDeleteModeChanged(bool isEnabled);
    void onHandlePointSpitEnabledChanged(bool isEnabled);
    void onHandlePointAdsorbEnabledChanged(bool isEnabled);
};

#endif // MAINWINDOW_H
