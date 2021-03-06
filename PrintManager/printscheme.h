#ifndef PRINTSCHEME_H
#define PRINTSCHEME_H

#include <QWidget>
#include <QFormLayout>
#include <QMap>
#include "FrontsDataStruct.h"


namespace Ui {
	class PrintScheme;
}

class PrintScheme : public QWidget
{
		Q_OBJECT

	public:
		explicit PrintScheme(FrontsDataStruct::Order order ,QString buyerAddress, int fontSize, int typeSize, int tableSize,
							 int descriptionSize,QWidget *parent = nullptr);
		~PrintScheme();

	private:
		Ui::PrintScheme *ui;
		QFormLayout *_mainLayout;
		FrontsDataStruct::Order _order;
		const int _fontSize;
		const int _typeSize;
		const int _tableSize;
		int _descSize;

		void header(QString address);
		void createTables();
};

#endif // PRINTSCHEME_H
