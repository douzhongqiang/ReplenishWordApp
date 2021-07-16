#ifndef REPLENISHWORDMAINWIDGET_H
#define REPLENISHWORDMAINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "FreeTypeRenderWidget.h"

class ReplenishWordMainWidget : public QWidget
{
    Q_OBJECT

public:
    ReplenishWordMainWidget(QWidget* parent = nullptr);
    ~ReplenishWordMainWidget();

private:
    FreeTypeRenderWidget* m_pReplenishWidget = nullptr;
    QLineEdit* m_pInputLineEdit = nullptr;

    void initUI(void);
    QWidget* createBottomWidget(void);

private slots:
    void onClickedButton(void);
};

#endif
