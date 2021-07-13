#ifndef REPLENISHWORDMAINWIDGET_H
#define REPLENISHWORDMAINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class ReplenishWordMainWidget : public QWidget
{
    Q_OBJECT

public:
    ReplenishWordMainWidget(QWidget* parent = nullptr);
    ~ReplenishWordMainWidget();

private:
    QWidget* m_pReplenishWidget = nullptr;
    QLineEdit* m_pInputLineEdit = nullptr;

    void initUI(void);
    QWidget* createBottomWidget(void);
};

#endif
