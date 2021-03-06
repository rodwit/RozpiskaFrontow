#include "printmanager.h"
#include "ui_printmanager.h"

#include "PrintManager/printscheme.h"
#include <QScreen>



PrintManager::PrintManager(QWidget *parent,FrontsDataStruct::Order order, QString buyerAddress) :
	QDialog(parent),
	ui(new Ui::PrintManager), _order(order), _address(buyerAddress),
	SCREEN_SIZE(QGuiApplication::primaryScreen()->geometry())
{
	ui->setupUi(this);



	// 0 == not change, get from FontSize
	ui->comboBoxDescriptionSize->addItem(QString::number(0));
	ui->comboBoxTableSize->addItem(QString::number(0));
	ui->comboBoxTypeSize->addItem(QString::number(0));
	for(int i= 26; i < 73; ++i)
	{
		ui->comboBoxTableSize->addItem(QString::number(i));
		ui->comboBoxDescriptionSize->addItem(QString::number(i));
		ui->comboBoxTypeSize->addItem(QString::number(i));
		ui->comboBoxFontSize->addItem(QString::number(i));
	}

	ui->comboBoxFontSize->setCurrentIndex(29);

	generate();

}

PrintManager::~PrintManager()
{
	delete ui;
}

void PrintManager::generate()
{
	int fontSize = ui->comboBoxFontSize->currentText().toInt();
	int typeSize = ui->comboBoxTypeSize->currentText().toInt();
	int tableSize = ui->comboBoxTableSize->currentText().toInt();
	int descSize = ui->comboBoxDescriptionSize->currentText().toInt();
	PrintScheme scheme(_order,_address, fontSize,typeSize,tableSize,descSize, this);
	_imageOrder = QPixmap(2480,3508); //2480,3508 A4 size
	QPainter painter(&_imageOrder);
	scheme.render(&painter);

	int widthScale = SCREEN_SIZE.width() * 0.75;
	int heightScale = SCREEN_SIZE.height() * 0.75;
	QPixmap scaledImageOrder = _imageOrder.scaled(widthScale,heightScale,Qt::KeepAspectRatio,Qt::SmoothTransformation);

	ui->labelPreview->setPixmap(scaledImageOrder);

	//debug
	//_imageOrder.save("TestImage.png");

}

void PrintManager::on_comboBoxFontSize_currentIndexChanged(const QString &arg1)
{
	generate();
}

void PrintManager::on_comboBoxTypeSize_currentIndexChanged(const QString &arg1)
{
	generate();
}

void PrintManager::on_comboBoxTableSize_currentIndexChanged(const QString &arg1)
{
	generate();
}

void PrintManager::on_comboBoxDescriptionSize_currentIndexChanged(const QString &arg1)
{
	generate();
}

void PrintManager::on_buttonBox_accepted()
{
	QPrinter printer(QPrinter::HighResolution);
	printer.setResolution(300);
	printer.setPageMargins(10,10,30,10, QPrinter::Millimeter);
	printer.setPaperSize(QPrinter::A4);
	QPainter painter(&printer);
	painter.drawPixmap(QPoint(0,0),_imageOrder);
	painter.end();
}
