#include "pagelist.h"
#include "ui_pagelist.h"

#include "MainWindow/mainwindow.h"
#include <QMenu>

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include "mytable.h"

PageList::PageList(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PageList)
{
	ui->setupUi(this);

	QObject::connect(ui->tableWidgetFronts, &QTableWidget::itemChanged,this, &PageList::updateCounter);
	connect(ui->tableWidgetFronts, &MyTable::TabKeySignal, this, &PageList::ActionAdd);

}

PageList::~PageList()
{
	delete ui;
}

void PageList::Clear()
{
	ui->lineEditType->clear();
	ui->checkBoxCount->setChecked(false);
	ui->checkBoxCount->setEnabled(false);
	ui->checkBoxCountM2->setChecked(false);
	ui->checkBoxCountM2->setEnabled(false);
	ui->checkBoxUnderline->setChecked(false);
	ui->checkBoxUnderline->setEnabled(false);
	ui->labelFrontsCount->setText("Ilość frontów: 0");
	int count = ui->tableWidgetFronts->rowCount();
	for(int i=0;  i < count; ++i)
		ui->tableWidgetFronts->removeRow(0);
}

FrontsDataStruct::FrontsTypeList PageList::GetFrontsList()
{
	FrontsDataStruct::FrontsTypeList frontsList;
	frontsList.attributes.insert(FrontsDataStruct::FrontsTypeList::AMOUNT_BOOL,ui->checkBoxCount->isChecked());
	frontsList.attributes.insert(FrontsDataStruct::FrontsTypeList::M2_BOOL,ui->checkBoxCountM2->isChecked());
	frontsList.attributes.insert(FrontsDataStruct::FrontsTypeList::UNDERLINE_BOOL,ui->checkBoxUnderline->isChecked());
	frontsList.type = ui->lineEditType->text();

	QList<FrontsDataStruct::Front> tempList;
	for (int i=0 ;i < ui->tableWidgetFronts->rowCount(); ++i)
	{
		FrontsDataStruct::Front tempFront;
		int height = ui->tableWidgetFronts->item(i,0)->text().toInt();
		int width = ui->tableWidgetFronts->item(i,1)->text().toInt();
		int amount = ui->tableWidgetFronts->item(i,2)->text().toInt();
		QString desc = ui->tableWidgetFronts->item(i,3)->text();

		tempFront.height = height;
		tempFront.width = width;
		tempFront.amount = amount;
		tempFront.description = desc;
		tempList.append(tempFront);
	}

	frontsList.list = tempList;

	return frontsList;

}

void PageList::LoadFronts(FrontsDataStruct::FrontsTypeList frontsList)
{
	for(auto front : frontsList.list)
		addFront(front);

	//after fronts because checkbox's
	ui->lineEditType->setText(frontsList.type);

	ui->checkBoxCount->setChecked(frontsList.attributes.value(FrontsDataStruct::FrontsTypeList::AMOUNT_BOOL).toBool());
	ui->checkBoxCountM2->setChecked(frontsList.attributes.value(FrontsDataStruct::FrontsTypeList::M2_BOOL).toBool());
	ui->checkBoxUnderline->setChecked(frontsList.attributes.value(FrontsDataStruct::FrontsTypeList::UNDERLINE_BOOL).toBool());
}

void PageList::ActionEdit()
{
	auto item = ui->tableWidgetFronts->currentItem();
	if(item)
		on_tableWidgetFronts_cellDoubleClicked(item->row(),item->column());
}

MainWindow* PageList::getParent() const
{
	return qobject_cast<MainWindow*>(this->parent()->parent()->parent()); // stack -> central -> main
}

void PageList::changefrontsNumber(int value, char sign)
{
	int frontsNumber = ui->labelFrontsCount->text().toInt();
	switch(sign)
	{
		case '=':
			frontsNumber = value;
			break;
		case '-':
			frontsNumber -= value;
			break;
		case '+':
			frontsNumber += value;
			break;
		default:
			return;
	}
	ui->labelFrontsCount->setText("Ilość frontów: " + QString::number(frontsNumber));
}

void PageList::updateCounter()
{
	int amount = 0;
	for (int i=0;i < ui->tableWidgetFronts->rowCount(); ++i)
	{
		if(ui->tableWidgetFronts->item(i,2) == nullptr)
			return;
		amount += ui->tableWidgetFronts->item(i,2)->text().toInt();
	}
	ui->labelFrontsCount->setText("Ilość frontów: " + QString::number(amount));
}

bool PageList::checkIfDuplicated(FrontsDataStruct::Front front, int updatedRow)
{
	//check if duplicat
	int rowNumber = ui->tableWidgetFronts->rowCount();

	int duplicateIndex = -1;
	for (int i = 0; i < rowNumber; ++i)
	{
		int checkedHeight = ui->tableWidgetFronts->item(i,0)->text().toInt();
		int checkedWidth = ui->tableWidgetFronts->item(i,1)->text().toInt();
		QString checkedText = "";
		if(ui->tableWidgetFronts->item(i,3))
			checkedText = ui->tableWidgetFronts->item(i,3)->text();

		if(updatedRow == i) //no check yourself
			continue;

		if( (front.height == checkedHeight) && (front.width == checkedWidth) && (checkedText == front.description ))
		{
			duplicateIndex = i;
			break;
		}
	}
	if(duplicateIndex != -1)
	{
		QMessageBox messageBox(QMessageBox::Warning,"Wykryto duplikat","Dodawany front już istnieje!<br>Czy połączyć oba fronty?<br><b>( " + QString::number(front.height) +" x "+ QString::number(front.width) + " = "+ QString::number(front.amount)+")</b>",QMessageBox::Yes | QMessageBox::No,this);
		//change color of duplicated row
		ui->tableWidgetFronts->item(duplicateIndex,0)->setBackground(QColor(255,255,102));
		ui->tableWidgetFronts->item(duplicateIndex,1)->setBackground(QColor(255,255,102));
		ui->tableWidgetFronts->item(duplicateIndex,2)->setBackground(QColor(255,255,102));
		ui->tableWidgetFronts->item(duplicateIndex,3)->setBackground(QColor(255,255,102));
		if(messageBox.exec() == QMessageBox::Yes)
		{
			auto duplicate = ui->tableWidgetFronts->item(duplicateIndex,2);
			duplicate->setText(QString::number((duplicate->text().toInt() + front.amount)));

			//remove old cell if calling from update
			if(updatedRow != -1)
			{
				ui->tableWidgetFronts->removeRow(updatedRow);
				updateCounter();
			}
		}
		//change color of duplicated row to normal
		ui->tableWidgetFronts->item(duplicateIndex,0)->setBackground(QColor(255,255,255));
		ui->tableWidgetFronts->item(duplicateIndex,1)->setBackground(QColor(255,255,255));
		ui->tableWidgetFronts->item(duplicateIndex,2)->setBackground(QColor(255,255,255));
		ui->tableWidgetFronts->item(duplicateIndex,3)->setBackground(QColor(255,255,255));

		return false;
	}
	return true;
}

void PageList::addFront(FrontsDataStruct::Front front, int insert)
{
	int index = insert;
	if(index == -1) //check if insert mode
	{
		ui->tableWidgetFronts->insertRow(ui->tableWidgetFronts->rowCount());
		index = ui->tableWidgetFronts->rowCount(); // last row
		if(index> 0)
			index-=1;
	}
	else
		ui->tableWidgetFronts->insertRow(index);

	ui->tableWidgetFronts->setItem(index,0,new QTableWidgetItem(QString::number( front.height)));
	ui->tableWidgetFronts->setItem(index,1,new QTableWidgetItem(QString::number(front.width)));
	ui->tableWidgetFronts->setItem(index,2,new QTableWidgetItem(QString::number(front.amount)));
	ui->tableWidgetFronts->setItem(index,3,new QTableWidgetItem(front.description));

	if(ui->checkBoxCountM2->isEnabled() == false && ui->tableWidgetFronts->rowCount() > 0)
	{
		ui->checkBoxCountM2->setEnabled(true);
		ui->checkBoxCountM2->setChecked(true);
		ui->checkBoxUnderline->setEnabled(true);
	}

	if(ui->checkBoxCount->isEnabled() == false && ui->tableWidgetFronts->rowCount() >= 2)
	{
		ui->checkBoxCount->setEnabled(true);
		ui->checkBoxCount->setChecked(true);
	}

}

void PageList::contextMenuEvent(QContextMenuEvent *event)
{
	if(!ui->tableWidgetFronts->underMouse())
		return;

	QMenu menu(this);
	auto item = ui->tableWidgetFronts->itemAt(ui->tableWidgetFronts->viewport()->mapFromGlobal(event->globalPos()));
	menu.addAction("Dodaj",this,&PageList::on_pushButtonAdd_clicked);
	if(item)
	{
		menu.addAction("Wstaw",this,&PageList::ActionInsert);
		menu.addAction("Edytuj",this,&PageList::ActionEdit);
		menu.addAction("Usuń",this,&PageList::ActionDelete);
	}
	menu.exec(event->globalPos());
}

void PageList::on_pushButtonAdd_clicked()
{
	DialogFront front(this);
	if(!front.exec())
		return;

	FrontsDataStruct::Front newFront =  front.getFront();

	if(checkIfDuplicated(newFront) == false)
		return;


	addFront(newFront);
}

void PageList::on_pushButtonInsert_clicked()
{
	DialogFront front(this);
	if(!front.exec())
		return;

	FrontsDataStruct::Front newFront =  front.getFront();

	if(checkIfDuplicated(newFront) == false)
		return;

	addFront(newFront,ui->tableWidgetFronts->currentRow());
}

void PageList::on_pushButtonDelete_clicked()
{
	if(ui->tableWidgetFronts->currentRow() < 0)
		return;

	auto item = ui->tableWidgetFronts->item(ui->tableWidgetFronts->currentRow(),2);

	if(item)
	{
		ui->tableWidgetFronts->removeRow(ui->tableWidgetFronts->currentRow());
		updateCounter();
	}
}

void PageList::on_tableWidgetFronts_cellDoubleClicked(int row, int column)
{
	int height = ui->tableWidgetFronts->item(row,0)->text().toInt();
	int width = ui->tableWidgetFronts->item(row,1)->text().toInt();
	int count = ui->tableWidgetFronts->item(row,2)->text().toInt();
	QString description = ui->tableWidgetFronts->item(row,3)->text();
	DialogFront front(this,height,width, count,description);

	if(!front.exec())
		return;

	FrontsDataStruct::Front updatedFront = front.getFront();

	if(checkIfDuplicated(updatedFront,row) == false)
		return;

	ui->tableWidgetFronts->item(row,0)->setText(QString::number(updatedFront.height));
	ui->tableWidgetFronts->item(row,1)->setText(QString::number(updatedFront.width));
	ui->tableWidgetFronts->item(row,2)->setText(QString::number(updatedFront.amount));
	ui->tableWidgetFronts->item(row,3)->setText(updatedFront.description);
}


void PageList::on_buttonBox_accepted()
{
	if(ui->lineEditType->text().isEmpty())
	{
		QMessageBox mb(QMessageBox::Warning,"Nieprawidłowe dane","Pole typ frontu nie może być puste",QMessageBox::Ok);
		mb.exec();
		return;
	}

	emit ListResult(true);
	getParent()->ChangePage(MainWindow::PAGES::ORDER);
}

void PageList::on_buttonBox_rejected()
{
	emit ListResult(false);
	getParent()->ChangePage(MainWindow::PAGES::ORDER);
}
