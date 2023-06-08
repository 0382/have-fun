#include "widget.h"
#include "screenwidget.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont("Microsoft Yahei", 9));

    auto w = new Widget();
    w->show();

    return a.exec();
}
