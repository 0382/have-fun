#include "widget.h"
#include <QApplication>
#include <QPushButton>
#include <QDateTime>
#include <QLabel>

#define STRDATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"))


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setGeometry(500, 500, 500, 500);
    screen = ScreenWidget::Instance();

    auto pb = new QPushButton(tr("矩形截图"), this);
    connect(pb, &QPushButton::clicked, this, &Widget::doGrab);
    connect(screen, &ScreenWidget::finished, this, &Widget::saveGrab);
}

Widget::~Widget()
{
}

void Widget::doGrab(){
    this->showMinimized();
    screen->show();
}

void Widget::saveGrab()
{
    this->showNormal();
//    QString fileName = QString("%1/screen_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
    map = screen->getGrab();
    auto *lb = new QLabel(this);
    lb->setPixmap(map);
    lb->setGeometry(QRect(50, 50, 450, 450));
    lb->show();
//    map.save(fileName, "PNG");
//    qDebug() << fileName << '\n';
}
