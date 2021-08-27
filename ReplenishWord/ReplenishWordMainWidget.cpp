#include "ReplenishWordMainWidget.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QChar>
#include <QDebug>
#include "HPNearCharacterCore.h"

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

    // Init WordSplit Core
    g_HPNearCharactorCore->init();

    // Add Bottom Widget
    QWidget* pButtomWidget = createBottomWidget();
    mainLayout->addWidget(pButtomWidget);
}

QWidget* ReplenishWordMainWidget::createBottomWidget(void)
{
    QWidget *pBottomWidget = new QWidget();
    QVBoxLayout* pMainLayout = new QVBoxLayout(pBottomWidget);
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(2);

    // Top Widget
    QWidget* pTopWidget = new QWidget;
    pMainLayout->addWidget(pTopWidget);

    QHBoxLayout* pBottomLayout = new QHBoxLayout(pTopWidget);
    pBottomLayout->setMargin(0);
    pBottomLayout->setSpacing(2);

    QLabel* pTagLabel = new QLabel(tr("Input Word:"));
    pBottomLayout->addWidget(pTagLabel);

    m_pInputLineEdit = new QLineEdit;
    pBottomLayout->addWidget(m_pInputLineEdit);

    QPushButton* pButton = new QPushButton(tr("Inset"));
    QObject::connect(pButton, &QPushButton::clicked, this, &ReplenishWordMainWidget::onClickedButton);
    pBottomLayout->addWidget(pButton);

    // Bottom Widget
    QWidget *pBottomWidget2 = new QWidget;
    pMainLayout->addWidget(pBottomWidget2);
    QHBoxLayout* pBottomLayout2 = new QHBoxLayout(pBottomWidget2);
    pBottomLayout2->setMargin(0);
    pBottomLayout2->setSpacing(2);

    m_pTextEdit = new QTextEdit;
    QFont font = m_pTextEdit->font();
    font.setPixelSize(20);
    m_pTextEdit->setFont(font);
    m_pTextEdit->setFixedHeight(120);
    pBottomLayout2->addWidget(m_pTextEdit);

    // Buttons
    QWidget* pButtonWdiget = new QWidget;
    pBottomLayout2->addWidget(pButtonWdiget);
    QVBoxLayout* pButtonLayout = new QVBoxLayout(pButtonWdiget);
    pButtonLayout->setMargin(0);
    pButtonLayout->setSpacing(0);

    QPushButton* pAddToTopButton = new QPushButton(tr("Add To Top"));
    pButtonLayout->addWidget(pAddToTopButton);

    QPushButton* pAddToBottomButton = new QPushButton(tr("Add To Bottom"));
    pButtonLayout->addWidget(pAddToBottomButton);

    return pBottomWidget;
}

void ReplenishWordMainWidget::onClickedButton(void)
{
    QString str = m_pInputLineEdit->text();
    m_pReplenishWidget->setCurrentRender(str);

    QVector<quint32> unicodes = g_HPNearCharactorCore->findWordByRadicals(str.unicode()->unicode());
    QString tempString;
    for (int i=0; i<unicodes.size(); ++i)
    {
        uint code = unicodes[i];
        QChar tempChar(code);
        tempString.push_back(tempChar);
    }

    m_pTextEdit->setPlainText(tempString);
}

void ReplenishWordMainWidget::setCurrentOperatorMode(FreeTypeRenderWidget::OperatorType type)
{
    m_pReplenishWidget->setCurrentOperatorTool(type);
}

void ReplenishWordMainWidget::clearSelectedItem(void)
{
    m_pReplenishWidget->clearSelectedItem();
}
