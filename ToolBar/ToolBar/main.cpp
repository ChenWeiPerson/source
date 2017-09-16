#include "toolbar.h"
#include <QtWidgets/QApplication>
#include <QFile>

class CommonHelper
{
public:
	static void setStyle(const QString &style) {
		QFile qss(style);
		if ( qss.open(QFile::ReadOnly) )
		{
			qApp->setStyleSheet(qss.readAll());
		}
		qss.close();
	}
};

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CommonHelper::setStyle("qss/psblack.css");
	ToolBar w;
	w.show();
	w.initContent();
	return a.exec();
}
