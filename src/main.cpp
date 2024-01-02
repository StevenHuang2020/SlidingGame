#include "game.h"
#include "vld.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    Game w;
    w.show();
    return a.exec();
}
