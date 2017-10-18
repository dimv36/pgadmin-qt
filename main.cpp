#include "pgadmin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	PgAdmin widget;
	widget.show();

	return app.exec();
}
