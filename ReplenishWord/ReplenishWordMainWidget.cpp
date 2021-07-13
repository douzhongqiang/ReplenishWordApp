#include "ReplenishWordMainWidget.h"
#include <QVBoxLayout>

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

    m_pReplenishWidget = new QWidget;
    m_pReplenishWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(m_pReplenishWidget);

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
    pBottomLayout->addWidget(pButton);

    return pBottomWidget;
}
