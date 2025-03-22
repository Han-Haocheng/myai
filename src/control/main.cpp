#include "MyaiController.h"
#include <QApplication>
#include <qpushbutton.h>
#include <qfont.h>

int main(int argc, char *argv[]) {
	QApplication a( argc, argv );

	QPushButton quit( "Quit", 0 );
	quit.resize( 75, 30 );
	quit.setFont(QFont("Times",18,QFont::Bold));

	QObject::connect(&quit,SIGNAL(chicked(bool)),&a,SLOT(quit()));
	quit.show();
	return a.exec();

}

