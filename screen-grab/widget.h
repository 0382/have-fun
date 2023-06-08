#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "screenwidget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void doGrab();
    void saveGrab();

private:
    ScreenWidget *screen = nullptr;
    QPixmap map;
};
#endif // WIDGET_H
