#include <QtGui/QApplication>
#include "qbeer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qbeer w;
    w.show();
    return a.exec();
}
