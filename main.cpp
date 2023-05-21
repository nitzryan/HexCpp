#include "HexCpp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HexCpp w;
    w.show();
    return a.exec();
}
