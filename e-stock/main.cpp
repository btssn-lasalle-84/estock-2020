#include "Ihm.h"
#include <QApplication>

/**
 * @file main.cpp
 *
 * @brief Programme prinicpal e-stock
 *
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ihm w;

    w.show();

    return a.exec();
}
