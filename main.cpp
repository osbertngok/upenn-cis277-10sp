/*  main is not doing much.
1)  Create a QApplication, passing along command line arguments
2)  Create an OpenGLTest instance, called w
3)  Show w, which is not enabled by default
4)  Call a.exec(), returning its return value when the user closes it
*/

#include "CIS277window.h"
#include <QtWidgets/QApplication>
using std::cout;
using std::endl;

int main(int argc, char *argv[])
{

	QApplication a(argc, argv);
	CIS277window w;
	w.show();
	return a.exec();
}
