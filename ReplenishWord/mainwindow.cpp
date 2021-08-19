#include "mainwindow.h"
#include "FreeTypeConfig.h"
#include "FreeTypeTool.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_pMainWidget = new ReplenishWordMainWidget;
    this->setCentralWidget(m_pMainWidget);

    initToolBar();
    initConnect();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initToolBar(void)
{
    m_pToolBar = new QToolBar(this);
    this->addToolBar(Qt::TopToolBarArea, m_pToolBar);

    QWidget* pWidegt = new QWidget;
    pWidegt->setMinimumHeight(40);
    pWidegt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pToolBar->addWidget(pWidegt);

    // Set Layout
    QHBoxLayout* pLayout = new QHBoxLayout(pWidegt);
    pLayout->setSpacing(1);
    pLayout->setMargin(0);

    // Select
    m_pSelectedButton = new QToolButton;
    pLayout->addWidget(m_pSelectedButton);
    m_pSelectedButton->setToolTip(tr("Move"));
    m_pSelectedButton->setFixedSize(32, 32);
    m_pSelectedButton->setCheckable(true);
    m_pSelectedButton->setChecked(true);
    m_pSelectedButton->setIcon(QIcon(":/images/SelectTool.png"));
    m_pSelectedButton->setIconSize(QSize(30, 30));
    QObject::connect(m_pSelectedButton, &QToolButton::clicked, this, &MainWindow::onMoveButtonCheckedStatusChanged);

    // Add Split Widget
    QWidget* pSplitWidget = new QWidget;
    pSplitWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    pSplitWidget->setFixedSize(2, 36);
    g_FreeTypeTool->applyCurrentStyle(pSplitWidget, "SplitStyle");
    pLayout->addSpacing(2);
    pLayout->addWidget(pSplitWidget);
    pLayout->addSpacing(2);

    // Handle
    m_pHandleButton = new QToolButton;
    pLayout->addWidget(m_pHandleButton);
    m_pHandleButton->setToolTip(tr("Handle"));
    m_pHandleButton->setCheckable(true);
    m_pHandleButton->setFixedSize(32, 32);
    m_pHandleButton->setIcon(QIcon(":/images/PenTool.png"));
    m_pHandleButton->setIconSize(QSize(30, 30));
    QObject::connect(m_pHandleButton, &QToolButton::clicked, this, &MainWindow::onHandleButtonCheckedStatusChanged);

    // Delete Handle
    m_pHandleDeleteButton = new QToolButton;
    pLayout->addWidget(m_pHandleDeleteButton);
    m_pHandleDeleteButton->setToolTip(tr("Delete Handle"));
    m_pHandleDeleteButton->setCheckable(true);
    m_pHandleDeleteButton->setFixedSize(32, 32);
    m_pHandleDeleteButton->setIcon(QIcon(":/images/Pen_Remove.png"));
    m_pHandleDeleteButton->setIconSize(QSize(30, 30));
    QObject::connect(m_pHandleDeleteButton, &QToolButton::clicked, this, &MainWindow::onDeleteButtonCheckedStatusChanged);

    // Spit
    m_pHandleSplitButton = new QToolButton;
    pLayout->addWidget(m_pHandleSplitButton);
    m_pHandleSplitButton->setToolTip(tr("Spit Handle"));
    m_pHandleSplitButton->setCheckable(true);
    m_pHandleSplitButton->setFixedSize(32, 32);
    m_pHandleSplitButton->setIcon(QIcon(":/images/Pen_Add.png"));
    m_pHandleSplitButton->setIconSize(QSize(30, 30));
    QObject::connect(m_pHandleSplitButton, &QToolButton::clicked, this, &MainWindow::onSpitButtonCheckedStatusChanged);

    // Adsorb
    m_pHandleAdsorbButton = new QToolButton;
    pLayout->addWidget(m_pHandleAdsorbButton);
    m_pHandleAdsorbButton->setToolTip(tr("Adsorb Handle"));
    m_pHandleAdsorbButton->setCheckable(true);
    m_pHandleAdsorbButton->setFixedSize(32, 32);
    m_pHandleAdsorbButton->setIcon(QIcon(":/images/Pen_adsorb.png"));
    m_pHandleAdsorbButton->setIconSize(QSize(30, 30));
    QObject::connect(m_pHandleAdsorbButton, &QToolButton::clicked, this, &MainWindow::onAdsorbButtonChecledStatusChanged);

    pLayout->addStretch();
}

void MainWindow::initConnect(void)
{
    QObject::connect(g_FreeTypeConfig, &FreeTypeConfig::handleEnabledChanged, this, &MainWindow::onHandleEnabledChanged);
    QObject::connect(g_FreeTypeConfig, &FreeTypeConfig::handlePointDeleteModeChanged, this, &MainWindow::onHandlePointDeleteModeChanged);
    QObject::connect(g_FreeTypeConfig, &FreeTypeConfig::handlePointSpitEnabledChanged, this, &MainWindow::onHandlePointSpitEnabledChanged);
    QObject::connect(g_FreeTypeConfig, &FreeTypeConfig::handlePointAdsorbEnabledChanged, this, &MainWindow::onHandlePointAdsorbEnabledChanged);
}

void MainWindow::onMoveButtonCheckedStatusChanged(void)
{
    m_pMainWidget->setCurrentOperatorMode(FreeTypeRenderWidget::t_MoveOperator);

    QToolButton* pButton = qobject_cast<QToolButton*>(sender());
    if (!pButton)
        return;

    g_FreeTypeConfig->setHandleEnabled(!pButton->isChecked());
}

void MainWindow::onHandleButtonCheckedStatusChanged(void)
{
    QToolButton* pButton = qobject_cast<QToolButton*>(sender());
    if (!pButton)
        return;

    g_FreeTypeConfig->setHandleEnabled(pButton->isChecked());
}

void MainWindow::onSpitButtonCheckedStatusChanged(void)
{
    QToolButton* pButton = qobject_cast<QToolButton*>(sender());
    if (!pButton)
        return;

    g_FreeTypeConfig->setHandlePointSpitEnabled(pButton->isChecked());
}

void MainWindow::onDeleteButtonCheckedStatusChanged(void)
{
    QToolButton* pButton = qobject_cast<QToolButton*>(sender());
    if (!pButton)
        return;

    g_FreeTypeConfig->setHandlePointDeleteMode(pButton->isChecked());
}

void MainWindow::onAdsorbButtonChecledStatusChanged(void)
{
    QToolButton* pButton = qobject_cast<QToolButton*>(sender());
    if (!pButton)
        return;

    g_FreeTypeConfig->setHandlePointAdsorbEnabled(pButton->isChecked());
}


void MainWindow::onHandleEnabledChanged(bool isEnabled)
{
    if (isEnabled)
    {
        m_pSelectedButton->setChecked(false);
        m_pHandleButton->setChecked(true);
    }
    else
    {
        m_pSelectedButton->setChecked(true);
        m_pHandleButton->setChecked(false);

        m_pHandleDeleteButton->setChecked(false);
        m_pHandleSplitButton->setChecked(false);
        m_pHandleAdsorbButton->setChecked(false);
    }
}

void MainWindow::onHandlePointDeleteModeChanged(bool isEnabled)
{
    if (isEnabled)
    {
        m_pHandleSplitButton->setChecked(false);
        m_pHandleAdsorbButton->setChecked(false);
    }
}

void MainWindow::onHandlePointSpitEnabledChanged(bool isEnabled)
{
    if (isEnabled)
    {
        m_pHandleDeleteButton->setChecked(false);
        m_pHandleAdsorbButton->setChecked(false);
    }
}

void MainWindow::onHandlePointAdsorbEnabledChanged(bool isEnabled)
{
    if (isEnabled)
    {
        m_pHandleDeleteButton->setChecked(false);
        m_pHandleSplitButton->setChecked(false);
    }
}
