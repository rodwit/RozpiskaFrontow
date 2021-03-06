#include "mytable.h"

//#include <QDebug>

MyTable::MyTable(QWidget * parent) : QTableWidget (parent)
{

}

void MyTable::keyPressEvent(QKeyEvent * e)
{
	/*if( (e->key() == Qt::Key::Key_Tab ) && (currentColumn() == (columnCount()-1)))
	{
		emit lastColumnPlusTabKeySignal();
		return;
	}*/
	if(e->key() == Qt::Key::Key_Tab)
	{
		emit TabKeySignal();
		return;
	}

	QTableWidget::keyPressEvent(e);
}

