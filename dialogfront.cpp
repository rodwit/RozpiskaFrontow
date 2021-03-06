#include "dialogfront.h"
#include "ui_dialogfront.h"

#include <QIntValidator>

DialogFront::DialogFront(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogFront)
{
	ui->setupUi(this);

}

DialogFront::DialogFront(QWidget *parent, int height, int width, int amount, QString description) : DialogFront(parent)
{
	ui->lineEditHeight->setText(QString::number(height));
	ui->lineEditWidth->setText(QString::number(width));
	ui->lineEditCount->setText(QString::number(amount));
	ui->lineEditDesc->setText(description);
}

DialogFront::~DialogFront()
{
	delete ui;
}

FrontsDataStruct::Front DialogFront::getFront()
{
	FrontsDataStruct::Front temp;
	temp.height = ui->lineEditHeight->text().toInt();
	temp.width = ui->lineEditWidth->text().toInt();
	temp.amount= ui->lineEditCount->text().toInt();
	temp.description = ui->lineEditDesc->text();
	return temp;
}

bool DialogFront::cellsValidate()
{
	QIntValidator val;
	val.setBottom(0);
	QString v1 = ui->lineEditHeight->text();
	QString v2 = ui->lineEditWidth->text();
	QString v3 = ui->lineEditDesc->text();
	int pos = 0;
	if(val.validate(v1,pos) == QIntValidator::Invalid || val.validate(v2,pos) == QIntValidator::Invalid)
		return true;
	// if empty cell
	if((ui->lineEditHeight->text() == "") || (ui->lineEditWidth->text() == "") || (ui->lineEditCount->text() == ""))
		return true;
	return false;


}

bool DialogFront::messageBox(QString text, QString informativeText, QMessageBox::StandardButton buttons, QMessageBox::StandardButton checked)
{
	QMessageBox mb;
	mb.setText(text);
	mb.setInformativeText(informativeText);
	mb.setStandardButtons(buttons);
	mb.setDefaultButton(checked);
	return mb.exec();
}

void DialogFront::accept()
{
	if(cellsValidate())
	{
		messageBox("Nieprawidłowe dane","Jedna z komórek posiada posiada nieprawidłowe dane",QMessageBox::Ok,QMessageBox::Ok);
		return;
	}

	QDialog::accept();
}
