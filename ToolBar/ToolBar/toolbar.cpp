#include <QTreeWidget>
#include <QListWidget>
#include <QPushbutton>
#include <QComboBox>
#include "toolbar.h"

ToolBar::ToolBar(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//this->setStyleSheet("QPushButton{background:yellow}QSlider{background:blue}");
	
}

ToolBar::~ToolBar()
{

}

void ToolBar::initContent()
{
	{
		QStringList list;
		list << "1" << "sda" << "dfd1";
		_ItemContent["1"] = list;
	}
	{
		QStringList list;
		list << "1"<<"12" << "sds" << "fdfd";
		_ItemContent["2"] = list;
	}
	{
		QStringList list;
		list << "1" << "fsd" << "xc" << "fgf" << "as" << "sdfs";
		_ItemContent["3"] = list;
	}
	{
		QStringList list;
		list << "1" << "gg" << "11" << "33" << "44";
		_ItemContent["4"] = list;
	}
	//{
	//	QStringList list;
	//	list << "1" << "gg" << "11" << "33" << "44";
	//	_ItemContent["button"] = list;
	//}
	for (std::map<QString, QStringList>::iterator iter = _ItemContent.begin();
		iter != _ItemContent.end(); ++iter)
	{
		QListWidget * w = new QListWidget;
		//w->setFrameShape(QFrame::NoFrame);
		w->addItems(iter->second);
		QSize s = w->size();
//		ui.treeWidget->AddItemWidget(iter->first, w);
	}
// 	ui.treeWidget->AddItemWidget("button", new QPushButton("button"));
// 	ui.treeWidget->AddItemWidget("QComboBox", new QComboBox);
}