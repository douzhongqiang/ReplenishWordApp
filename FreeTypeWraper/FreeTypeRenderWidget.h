#ifndef FREETYPERENDERWIDGET_H
#define FREETYPERENDERWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include <QGraphicsView>
#include "FreeTypeCore.h"
#include "FreeTypeWraper_global.h"
#include <QGraphicsScene>

class FreeTypeOperatorBase;
class FreeTypeSelectedItem;
class FREETYPEWRAPER_EXPORT FreeTypeRenderWidget : public QGraphicsView
{
    Q_OBJECT
public:
    enum OperatorType
    {
        t_MoveOperator,
        t_ResizeOperator,
        t_RotateOperator,
    };

public:
    FreeTypeRenderWidget(QWidget* parent = nullptr);
    ~FreeTypeRenderWidget();

    void loadCurrentFont(const QString& fontPath);
    void setCurrentRender(const QString& renderString);

    void setFreeTypeCore(QSharedPointer<FreeTypeCore> freeTypeCore);

    // Operator About
    void setOperator(FreeTypeOperatorBase* pOperator);

    // Set Select Rect About
    void setSelectRectEnabled(bool isEnabled);
    void setSelectRect(const QRect& rect);

    void drawBackground(QPainter * painter, const QRectF & rect) override;
    void drawForeground(QPainter * painter, const QRectF & rect) override;

    // Set / Get Current Operator Tool
    void setCurrentOperatorTool(OperatorType type);
    OperatorType getCurrentOperatorTool(void);

    // Get Selected Item Handle Rect
    FreeTypeSelectedItem* getScaledItemHandleItem(void);

    // Cursor About
    void setPenHandleCursor(const QCursor& cursor);
    QCursor getPenHandleCursor(void);
    void setRotateHandleCursor(const QCursor& cursor);
    QCursor getRotateHandleCursor(void);

    // Clear Select Item
    void clearSelectedItem(void);

private:
    QSharedPointer<FreeTypeCore> m_pFreeTypeCore;
    QGraphicsScene* m_pScene = nullptr;

    void initSceneItems(void);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    FreeTypeOperatorBase* m_pOperator = nullptr;

private:
    // Select Rect About
    QColor m_cSelectedPenColor;
    QColor m_cSelectedBrushColor;
    bool m_isSelectEnabled = false;
    QRect m_selectRect;
    void paintSelectRect(QPainter* painter);

    // Select Item About
    FreeTypeSelectedItem* m_pSelectItem = nullptr;

    OperatorType m_currentOperatorType;

    // Cursor About
    QCursor m_penHandleCursor;
    QCursor m_rotateHandleCursor;

public slots:
    void onItemSelectionChanged(void);
};

#endif
