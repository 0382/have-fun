#include "screenwidget.h"
#include <QApplication>
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QScreen>

ScreenWidget *ScreenWidget::Instance()
{
    static ScreenWidget *data = new ScreenWidget();
    return data;
}

ScreenWidget::ScreenWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->setStyleSheet("background:gray;");
    this->setWindowOpacity(0.5);
    auto geometry = QApplication::primaryScreen()->availableGeometry();
    this->setGeometry(geometry);
    this->setCursor(Qt::CrossCursor);
    black_mask = QBitmap(geometry.size());
    black_mask.fill(Qt::black);
    rect_mask = black_mask.copy();
}

void ScreenWidget::paintEvent(QPaintEvent *)
{
    auto area = make_Rect(start_point, end_point);
    int x = area.x();
    int y = area.y();
    int w = area.width();
    int h = area.height();

    if (w != 0 && h != 0) {
        rect_mask = black_mask.copy();
        QPainter painter(&rect_mask);
        QPen pen;
        pen.setStyle(Qt::NoPen);
        QBrush brush(Qt::white);
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.drawRect(area);
        this->setMask(QBitmap::fromPixmap(rect_mask));

        QPainter painter2(this);
        pen.setColor(Qt::green);
        pen.setWidth(2);
        pen.setStyle(Qt::DotLine);
        painter2.setPen(pen);
        painter2.drawRect(area);
        pen.setColor(Qt::cyan);
        painter2.setPen(pen);
        painter2.drawText(x + 2, y - 8, tr("截图范围：( %1 x %2 ) - ( %3 x %4 )  图片大小：( %5 x %6 )")
                         .arg(x).arg(y).arg(x + w).arg(y + h).arg(w).arg(h));
    }
}

void ScreenWidget::showEvent(QShowEvent *)
{
    start_point = QPoint(0, 0);
    end_point = QPoint(0, 0);
    this->setMask(black_mask);
}

QPixmap ScreenWidget::getGrab()
{
    auto area = make_Rect(this->start_point, this->end_point);
    auto geo = this->geometry();
    QScreen *pscreen = QApplication::primaryScreen();
    return pscreen->grabWindow(0, area.x() + geo.x(), area.y() + geo.y(), area.width(), area.height());
}


void ScreenWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(this->is_drawing)
    {
        this->end_point = e->position();
    }
    this->update();
}

void ScreenWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::MouseButton::LeftButton)
    {
        this->start_point = e->position();
        this->end_point = this->start_point;
        this->is_drawing = true;
    }
    this->update();
}

void ScreenWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::MouseButton::LeftButton)
    {
        this->is_drawing = false;
        this->end_point = e->position();
        this->close();
        emit finished();
    }
}
