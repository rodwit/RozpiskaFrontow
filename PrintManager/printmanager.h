#ifndef PRINTMANAGER_H
#define PRINTMANAGER_H

#include <QDialog>
#include "FrontsDataStruct.h"

#include <QPrinter>
#include <QPainter>

namespace Ui {
	class PrintManager;
}

class PrintManager : public QDialog
{
		Q_OBJECT

	public:
		explicit PrintManager(QWidget *parent, FrontsDataStruct::Order order, QString buyerAddress);
		~PrintManager();

	private slots:
		void on_comboBoxFontSize_currentIndexChanged(const QString &arg1);

		void on_comboBoxTypeSize_currentIndexChanged(const QString &arg1);

		void on_comboBoxTableSize_currentIndexChanged(const QString &arg1);

		void on_comboBoxDescriptionSize_currentIndexChanged(const QString &arg1);

		void on_buttonBox_accepted();

	private:
		const float WIDTH_PER = 0.75;
		const float HEIGHT_PER = 0.75;
		const QRect SCREEN_SIZE;

		Ui::PrintManager *ui;
		FrontsDataStruct::Order _order;
		QString _address;
		QPixmap _imageOrder;

		void generate();

};

#endif // PRINTMANAGER_H
