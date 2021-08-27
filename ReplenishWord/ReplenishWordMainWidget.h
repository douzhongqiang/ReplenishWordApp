#ifndef REPLENISHWORDMAINWIDGET_H
#define REPLENISHWORDMAINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include "FreeTypeRenderWidget.h"

class ReplenishWordMainWidget : public QWidget
{
    Q_OBJECT

public:
    ReplenishWordMainWidget(QWidget* parent = nullptr);
    ~ReplenishWordMainWidget();

    void setCurrentOperatorMode(FreeTypeRenderWidget::OperatorType type);

    void clearSelectedItem(void);

private:
    FreeTypeRenderWidget* m_pReplenishWidget = nullptr;
    QLineEdit* m_pInputLineEdit = nullptr;
    QTextEdit* m_pTextEdit = nullptr;

    void initUI(void);
    QWidget* createBottomWidget(void);

private slots:
    void onClickedButton(void);
};

#endif
