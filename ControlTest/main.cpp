#include "ControlTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ControlTest w;
    w.show();
    return a.exec();
}
