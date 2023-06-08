#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include <QWidget>
#include <QPoint>
#include <QSize>

//截屏窗口类
class ScreenWidget : public QWidget
{
    Q_OBJECT
signals:
    void finished(void);
public:
    static ScreenWidget *Instance();
    QPixmap getGrab();

private:
    explicit ScreenWidget(QWidget *parent = 0);

private:
    bool is_drawing = false;
    QPointF start_point, end_point;
    QBitmap black_mask;
    QPixmap rect_mask;

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *);

    static QRect make_Rect(QPointF p1, QPointF p2)
    {
        auto x1 = p1.x();
        auto x2 = p2.x();
        auto y1 = p1.y();
        auto y2 = p2.y();
        QRectF r = QRectF(QPointF(std::min(x1, x2), std::min(y1, y2)), QPointF(std::max(x1, x2), std::max(y1, y2)));
        return r.toRect();
    }
};
#endif // SCREENWIDGET_H
