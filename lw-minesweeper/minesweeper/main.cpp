#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	const QString &state_file = "gamestate.ini";
	std::string dbg = "";
	if (argc > 1)
	{
		dbg = argv[1];
	}
	w.restoreGame(state_file, dbg == "dbg" ? true : false);
	w.show();

	QObject::connect(&a, &QApplication::aboutToQuit, [&]() { w.saveGame(state_file); });
	return a.exec();
}
