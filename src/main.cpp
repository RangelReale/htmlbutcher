#include <QApplication>
#include <QMetaType>

#include "HTMLButcherMain.h"
#include "wxFreeImage.h"
#include "ButcherProjectEvent.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	qRegisterMetaType<ButcherProjectEvent>("ButcherProjectEvent");

	HTMLButcherFrame window;

	return app.exec();
}