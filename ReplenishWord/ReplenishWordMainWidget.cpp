#include "ReplenishWordMainWidget.h"
#include <QVBoxLayout>
#include <QApplication>

ReplenishWordMainWidget::ReplenishWordMainWidget(QWidget* parent)
    :QWidget(parent)
{
    initUI();
}

ReplenishWordMainWidget::~ReplenishWordMainWidget()
{

}

void ReplenishWordMainWidget::initUI(void)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    m_pReplenishWidget = new FreeTypeRenderWidget;
    m_pReplenishWidget->setPenHandleCursor(QCursor(QPixmap(":/images/SelectTool2.png").scaled(26, 26, Qt::IgnoreAspectRatio, Qt::SmoothTransformation), 2, 2));
    m_pReplenishWidget->setRotateHandleCursor(QPixmap(":/images/RotateTool.png"));
    m_pReplenishWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(m_pReplenishWidget);

    // Init Render Widget
    QString fontPath = qApp->applicationDirPath() + "/simsun.ttc";
    m_pReplenishWidget->loadCurrentFont(fontPath);

    // Add Bottom Widget
    QWidget* pButtomWidget = createBottomWidget();
    mainLayout->addWidget(pButtomWidget);
}

QWidget* ReplenishWordMainWidget::createBottomWidget(void)
{
    QWidget *pBottomWidget = new QWidget();
    QHBoxLayout* pBottomLayout = new QHBoxLayout(pBottomWidget);
    pBottomLayout->setMargin(0);
    pBottomLayout->setSpacing(2);

    QLabel* pTagLabel = new QLabel(tr("Input Word:"));
    pBottomLayout->addWidget(pTagLabel);

    m_pInputLineEdit = new QLineEdit;
    pBottomLayout->addWidget(m_pInputLineEdit);

    QPushButton* pButton = new QPushButton(tr("Inset"));
    QObject::connect(pButton, &QPushButton::clicked, this, &ReplenishWordMainWidget::onClickedButton);
    pBottomLayout->addWidget(pButton);

    return pBottomWidget;
}

void ReplenishWordMainWidget::onClickedButton(void)
{
    QString str = m_pInputLineEdit->text();
    m_pReplenishWidget->setCurrentRender(str);
}

void ReplenishWordMainWidget::setCurrentOperatorMode(FreeTypeRenderWidget::OperatorType type)
{
    m_pReplenishWidget->setCurrentOperatorTool(type);
}

void ReplenishWordMainWidget::clearSelectedItem(void)
{
    m_pReplenishWidget->clearSelectedItem();
}
