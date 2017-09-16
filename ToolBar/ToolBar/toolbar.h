#ifndef TOOLBAR_H
#define TOOLBAR_H
#include<map>
#include <QtWidgets/QMainWindow>
#include "ui_toolbar.h"

class ToolBar : public QMainWindow
{
	Q_OBJECT

public:
	ToolBar(QWidget *parent = 0);
	~ToolBar();
	void initContent();
private:
	Ui::ToolBarClass ui;
	std::map<QString, QStringList> _ItemContent;
};

#endif // TOOLBAR_H
