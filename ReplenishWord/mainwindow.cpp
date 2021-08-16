#include "mainwindow.h"
#include "FreeTypeConfig.h"

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

    // Select
    QToolButton* pSelectButton = new QToolButton;
    m_pToolBar->addWidget(pSelectButton);
    pSelectButton->setToolTip(tr("Move"));
    QObject::connect(pSelectButton, &QToolButton::clicked, this, &MainWindow::onMoveButtonCheckedStatusChanged);

    // Resize
    QToolButton* pResizeButton = new QToolButton;
    m_pToolBar->addWidget(pResizeButton);
    pResizeButton->setToolTip(tr("Resize"));
    QObject::connect(pResizeButton, &QToolButton::clicked, this, &MainWindow::onResizeButtonCheckedStatusChanged);

    // Rotate
    QToolButton* pRotateButton = new QToolButton;
    m_pToolBar->addWidget(pRotateButton);
    pRotateButton->setToolTip(tr("Rotate"));
    QObject::connect(pRotateButton, &QToolButton::clicked, this, &MainWindow::onRotateButtonCheckedStatusChanged);

    // Handle
    QToolButton* pHandleButton = new QToolButton;
    m_pToolBar->addWidget(pHandleButton);
    pHandleButton->setToolTip(tr("Handle"));
    pHandleButton->setCheckable(true);
    QObject::connect(pHandleButton, &QToolButton::clicked, this, &MainWindow::onHandleButtonCheckedStatusChanged);

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

void MainWindow::onHandleButtonCheckedStatusChanged(void)
{
    QToolButton* pButton = qobject_cast<QToolButton*>(sender());
    if (!pButton)
        return;

    g_FreeTypeConfig->setHandleEnabled(pButton->isChecked());
}
