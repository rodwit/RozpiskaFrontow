#ifndef MYTABLE_H
#define MYTABLE_H

#include <QTableWidget>
#include <QEvent>
#include <QFocusEvent>
#include <QKeyEvent>
class MyTable : public QTableWidget
{
		Q_OBJECT
	public:
		explicit MyTable(QWidget *parent = Q_NULLPTR);

	protected:
		virtual void keyPressEvent(QKeyEvent *e);


	signals:
		//void lastColumnPlusTabKeySignal();
		void TabKeySignal();

};

#endif // MYTABLE_H
