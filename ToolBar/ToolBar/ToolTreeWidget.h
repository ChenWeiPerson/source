#ifndef TOOLTREEWIDGET_H
#define TOOLTREEWIDGET_H

#include <QTreeWidget>

class ToolTreeWidget : public QTreeWidget
{
	Q_OBJECT

public:
	ToolTreeWidget(QWidget *parent);
	~ToolTreeWidget();
	void AddItemWidget( QString str,QWidget * w );
	void JustItemSize();	
	void JustItemSize(QTreeWidgetItem * item );
protected:

	virtual void
		resizeEvent(QResizeEvent *event);
private:
	
};

#endif // TOOLTREEWIDGET_H
