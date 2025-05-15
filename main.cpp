#include "ShipmentItems.h"
#include <QtWidgets/QApplication>
#include "UserOrders.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ShipmentItems w;
    w.show();
    return a.exec();
}
