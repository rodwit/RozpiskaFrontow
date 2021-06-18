#include "pageorder.h"
#include "ui_pageorder.h"

#include <QFile>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <PrintManager/printmanager.h>


#include "MainWindow/mainwindow.h"


PageOrder::PageOrder(QWidget *parent,PageList *pageList) :
	QWidget(parent),
	ui(new Ui::PageOrder)
{
	ui->setupUi(this);

	_pageList = pageList;

	_editList = false;

	_transmittedItem = -1;

	//_parent = qobject_cast<MainWindow*>(this->parent()->parent()->parent()); // stack -> central -> main

	//QObject::connect(ui->listWidget,&MyTable::TabKeySignal,this,&PageOrder::ActionAdd);
	//connect(ui->tableWidgetFronts, SIGNAL(lastColumnPlusTabKeySignal()), this, SLOT(lastColumnPlusTabKeySlot()));

	getSprayers();

	_isOrderUpdated = false;


}

PageOrder::~PageOrder()
{
	delete ui;
}



void PageOrder::Load(FrontsDataStruct::Order order)
{
	_order.name = order.name;
	ui->lineEditOrderName->setText(_order.name);
	int index = ui->comboBoxSprayers->findText(order.sprayer);
	ui->comboBoxSprayers->setCurrentIndex(index);
	for(auto ele : order.list)
		addFrontsList(ele);
}

void PageOrder::Clear()
{
	ui->lineEditOrderName->setText("");
	ui->listWidget->clear();
	_order = FrontsDataStruct::Order();
}

void PageOrder::GetListResult(bool value)
{
	if(value)
	{
		_isOrderUpdated = true;

		if(_editList)
		{
			_order.list.replace(ui->listWidget->currentRow(),_pageList->GetFrontsList());
			ui->listWidget->currentItem()->setText(_pageList->GetFrontsList().type);
		}
		else
			addFrontsList(_pageList->GetFrontsList());


	}

	_editList = false;
	_pageList->Clear();
}

void PageOrder::ActionEdit()
{
	auto item = ui->listWidget->currentItem();
	if(item)
		on_listWidget_itemDoubleClicked(item);
}

void PageOrder::getSprayers()
{
	QFile file(QApplication::applicationDirPath() + "/Sprayers.xml");
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(this,"Błąd pobierania danych", "Nie można pobrać listy lakierników");
		//qobject_cast<MainWindow*>(parent()->parent()->parent())->close();
		throw "Sprayer.xml expected";
		//QApplication::quit();
	}

	QXmlStreamReader reader(&file);

	while(!reader.atEnd() && !reader.hasError())
	{
		QXmlStreamReader::TokenType token = reader.readNext();

		if(token == QXmlStreamReader::StartDocument)
			continue;

		if(token == QXmlStreamReader::StartElement)
		{
			if(reader.name() == "Sprayer")
			{
				ui->comboBoxSprayers->addItem(reader.readElementText().simplified());
			}
			if(reader.name() == "Address")
				_address = reader.readElementText().simplified();
		}


	}
}

MainWindow *PageOrder::getParent() const
{
	return qobject_cast<MainWindow*>(this->parent()->parent()->parent()); // stack -> central -> main
}

void PageOrder::addFrontsList(FrontsDataStruct::FrontsTypeList list)
{
	_order.list.append(list);
	ui->listWidget->addItem(list.type);
	ui->listWidget->item(ui->listWidget->count() -1)->setTextAlignment(Qt::AlignCenter);
}

void PageOrder::on_pushButtonAdd_clicked()
{
	getParent()->ChangePage(getParent()->LIST);
}

void PageOrder::on_pushButtonDelete_clicked()
{
	int index = ui->listWidget->currentRow();
	if(index < 0)
		return;

	auto temp = ui->listWidget->takeItem(index);
	delete temp;

	_order.list.removeAt(index);
}

void PageOrder::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
	_editList = true;
	int index = ui->listWidget->currentRow();
	_pageList->LoadFronts(_order.list[index]);
	getParent()->ChangePage(MainWindow::LIST);
}

void PageOrder::on_pushButtonMove_clicked()
{
	_transmittedItem = ui->listWidget->currentRow();
}

void PageOrder::on_listWidget_itemClicked(QListWidgetItem *item)
{
	if(_transmittedItem != -1) //swap items
	{
		int currentIndex = ui->listWidget->currentRow();
		QString targetItemString = ui->listWidget->currentItem()->text();

		ui->listWidget->item(currentIndex)->setText(ui->listWidget->item(_transmittedItem)->text());
		ui->listWidget->item(_transmittedItem)->setText(targetItemString);

		_order.list.swap(_transmittedItem,currentIndex);

		_transmittedItem = -1;
	}
}

void PageOrder::on_pushButtonSave_clicked()
{
	_order.name = ui->lineEditOrderName->text();
	_order.sprayer = ui->comboBoxSprayers->currentText();
	if(getParent()->SaveOrder(_order))
		_isOrderUpdated = false;
}

void PageOrder::ActionSaveAs()
{
	_order.name = ui->lineEditOrderName->text();
	_order.sprayer = ui->comboBoxSprayers->currentText();
	if(getParent()->SaveOrder(_order,true))
		_isOrderUpdated = false;
}


void PageOrder::on_pushButtonPrint_clicked()
{
	_order.name = ui->lineEditOrderName->text();
	_order.sprayer = ui->comboBoxSprayers->currentText();
	PrintManager manager(this,_order,_address);
	manager.exec();
}

void PageOrder::on_pushButtonClose_clicked()
{
	qApp->closeAllWindows();
}
