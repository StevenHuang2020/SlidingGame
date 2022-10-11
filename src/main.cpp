#include "game.h"
#include <QtWidgets/QApplication>
#include "vld.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	Game w;
	w.show();
	return a.exec();
}
