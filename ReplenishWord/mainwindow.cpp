#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_pMainWidget = new ReplenishWordMainWidget;
    this->setCentralWidget(m_pMainWidget);

    initToolBar();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initToolBar(void)
{
    m_pToolBar = new QToolBar(this);
    this->addToolBar(Qt::TopToolBarArea, m_pToolBar);

    // Move
    QToolButton* pSelectButton = new QToolButton;
    m_pToolBar->addWidget(pSelectButton);
    pSelectButton->setToolTip(tr("Move"));
    pSelectButton->setStyleSheet("QToolButton{background-color: rgb(120, 120, 120); "
                                 "border: 1px solid rgb(200, 200, 200);}"
                                 "QToolButton:hover{background-color: rgb(150, 150, 150);}");
    QObject::connect(pSelectButton, &QToolButton::clicked, this, &MainWindow::onMoveButtonCheckedStatusChanged);

    // Resize
    QToolButton* pResizeButton = new QToolButton;
    m_pToolBar->addWidget(pResizeButton);
    pResizeButton->setToolTip(tr("Resize"));
    pResizeButton->setStyleSheet("QToolButton{background-color: rgb(120, 120, 120); "
                                 "border: 1px solid rgb(200, 200, 200);}"
                                 "QToolButton:hover{background-color: rgb(150, 150, 150);}");
    QObject::connect(pResizeButton, &QToolButton::clicked, this, &MainWindow::onResizeButtonCheckedStatusChanged);

    // Rotate
    QToolButton* pRotateButton = new QToolButton;
    m_pToolBar->addWidget(pRotateButton);
    pRotateButton->setToolTip(tr("Rotate"));
    pRotateButton->setStyleSheet("QToolButton{background-color: rgb(120, 120, 120); "
                                 "border: 1px solid rgb(200, 200, 200);}"
                                 "QToolButton:hover{background-color: rgb(150, 150, 150);}");
    QObject::connect(pRotateButton, &QToolButton::clicked, this, &MainWindow::onRotateButtonCheckedStatusChanged);

}

void MainWindow::onMoveButtonCheckedStatusChanged(void)
{
    m_pMainWidget->setCurrentOperatorMode(FreeTypeRenderWidget::t_MoveOperator);
}

void MainWindow::onResizeButtonCheckedStatusChanged(void)
{
    m_pMainWidget->setCurrentOperatorMode(FreeTypeRenderWidget::t_ResizeOperator);
}

void MainWindow::onRotateButtonCheckedStatusChanged(void)
{
    m_pMainWidget->setCurrentOperatorMode(FreeTypeRenderWidget::t_RotateOperator);
}
