#ifndef DIALOGFRONT_H
#define DIALOGFRONT_H

#include <QDialog>

#include "FrontsDataStruct.h"
#include <QMessageBox>

namespace Ui {
	class DialogFront;
}

class DialogFront : public QDialog
{
		Q_OBJECT

	public:
		explicit DialogFront(QWidget *parent = nullptr);
		explicit DialogFront(QWidget *parent, int height, int width, int amount, QString description);
		~DialogFront();

		FrontsDataStruct::Front getFront();
	private slots:
		void accept() override;

	private:
		Ui::DialogFront *ui;

		bool cellsValidate();
		bool messageBox(QString text, QString informativeText, QMessageBox::StandardButton buttons, QMessageBox::StandardButton checked);


};

#endif // DIALOGFRONT_H
