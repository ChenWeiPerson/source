#include <QHeaderView>
#include "sheet_delegate.h"
#include "ToolTreeWidget.h"

ToolTreeWidget::ToolTreeWidget(QWidget *parent)
	: QTreeWidget(parent)
{
	this->setIndentation(0);
	setItemDelegate(new qdesigner_internal::SheetDelegate(this, this));
	this->setRootIsDecorated(false);
}

ToolTreeWidget::~ToolTreeWidget()
{

}
void ToolTreeWidget::AddItemWidget(QString str, QWidget * w)
{
	QList<QTreeWidgetItem *>  items = this->findItems(str, Qt::MatchFixedString);
	if (items.isEmpty())
	{

		w->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
		QTreeWidgetItem * topItem = new QTreeWidgetItem(QStringList() << str);
		//topItem->setTextAlignment(0,Qt::AlignHCenter);
		this->addTopLevelItem(topItem);
		QTreeWidgetItem * childItem = new QTreeWidgetItem(topItem);
		this->setItemWidget(childItem, 0,w);
		JustItemSize(topItem);
	}
}

void ToolTreeWidget::JustItemSize()
{
	for (size_t i = 0; i < this->topLevelItemCount(); i++)
	{
		JustItemSize(this->topLevelItem(i));
	}
}

void ToolTreeWidget::JustItemSize(QTreeWidgetItem * item )
{
	if (QTreeWidgetItem * childItem = item->child(0) )
	{
		if (QWidget * w = qobject_cast<QWidget*>(this->itemWidget(childItem,0)))
		{
			childItem->setSizeHint(0, QSize(this->size().width(), w->size().height()));
		}
	}
}

void ToolTreeWidget::resizeEvent(QResizeEvent *event)
{
	QTreeWidget::resizeEvent(event);
	JustItemSize();
}