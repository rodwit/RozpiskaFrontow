#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <QDebug>
#include <QException>
#include <QFontDialog>
#include <QCloseEvent>
#include <QShowEvent>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	_settings = new QSettings(QCoreApplication::applicationDirPath()+ "/settings.ini",QSettings::IniFormat,this);
	QString lastSavePath = _settings->value(QVariant::fromValue(SETTINGS_KEYS::LAST_SAVE_PATH).toString()).toString();
	QString lastOpenPath = _settings->value(QVariant::fromValue(SETTINGS_KEYS::LAST_OPEN_PATH).toString()).toString();
	_fileData = new FileData(this,lastSavePath,lastOpenPath);
	_pageList = new PageList(this);
	_pageOrder = new PageOrder(this,_pageList);
	QObject::connect(_pageList,&PageList::ListResult,_pageOrder,&PageOrder::GetListResult);

	ui->stackedWidget->addWidget(_pageOrder);
	ui->stackedWidget->addWidget(_pageList);

	menuBar()->actions()[1]->setEnabled(false); //edit
	menuBar()->actions()[0]->menu()->actions()[2]->setEnabled(false); // save
	menuBar()->actions()[0]->menu()->actions()[3]->setEnabled(false); // save as
	menuBar()->actions()[0]->menu()->actions()[4]->setEnabled(false); // print


	//load font
	QFont font;
//	qDebug() << "Font: " + _settings->value("Font",QApplication::font()).toString();
	font.fromString(_settings->value("Font",QApplication::font()).toString());
	QApplication::setFont(font);

	//load order from argument
	if(QApplication::arguments().size() > 1)
		this->loadOrder(QApplication::arguments().at(1));

}

MainWindow::~MainWindow()
{
	delete _pageList;
	delete _pageOrder;
	delete _settings;
	delete _fileData;
	delete ui;
}

void MainWindow::ChangePage(MainWindow::PAGES page)
{
	switch (page)
	{
		case PAGES::MAIN:
			menuBar()->actions()[1]->setEnabled(false); //edit
			menuBar()->actions()[0]->menu()->actions()[2]->setEnabled(false); // save
			menuBar()->actions()[0]->menu()->actions()[3]->setEnabled(false); // save as
			menuBar()->actions()[0]->menu()->actions()[4]->setEnabled(false); // print
			ui->stackedWidget->setCurrentIndex(0);
			break;
		case PAGES::ORDER:
			QObject::connect(ui->actionAdd,&QAction::triggered,_pageOrder,&PageOrder::ActionAdd);
			QObject::connect(ui->actionSave,&QAction::triggered,_pageOrder,&PageOrder::ActionSave);
			QObject::connect(ui->actionDelete,&QAction::triggered,_pageOrder,&PageOrder::ActionDelete);
			QObject::connect(ui->actionEdit,&QAction::triggered,_pageOrder,&PageOrder::ActionEdit);
			QObject::connect(ui->actionPrint,&QAction::triggered,_pageOrder,&PageOrder::ActionPrint);

			QObject::disconnect(ui->actionAdd,&QAction::triggered,_pageList,&PageList::ActionAdd);
			QObject::disconnect(ui->actionDelete,&QAction::triggered,_pageList,&PageList::ActionDelete);
			QObject::disconnect(ui->actionEdit,&QAction::triggered,_pageList,&PageList::ActionEdit);

			menuBar()->actions()[1]->menu()->actions()[1]->setVisible(false); //insert
			menuBar()->actions()[1]->setEnabled(true); //edit
			menuBar()->actions()[0]->menu()->actions()[2]->setEnabled(true); // save
			menuBar()->actions()[0]->menu()->actions()[3]->setEnabled(true); // save as
			menuBar()->actions()[0]->menu()->actions()[4]->setEnabled(true); // print
			ui->stackedWidget->setCurrentWidget(_pageOrder);
			break;
		case PAGES::LIST:
			QObject::disconnect(ui->actionAdd,&QAction::triggered,_pageOrder,&PageOrder::ActionAdd);
			QObject::disconnect(ui->actionDelete,&QAction::triggered,_pageOrder,&PageOrder::ActionDelete);
			QObject::disconnect(ui->actionEdit,&QAction::triggered,_pageOrder,&PageOrder::ActionEdit);

			QObject::connect(ui->actionAdd,&QAction::triggered,_pageList,&PageList::ActionAdd);
			QObject::connect(ui->actionDelete,&QAction::triggered,_pageList,&PageList::ActionDelete);
			QObject::connect(ui->actionEdit,&QAction::triggered,_pageList,&PageList::ActionEdit);
			QObject::connect(ui->actionInsert,&QAction::triggered,_pageList,&PageList::ActionInsert);


			menuBar()->actions()[1]->menu()->actions()[1]->setVisible(true); //insert
			ui->stackedWidget->setCurrentWidget(_pageList);
			break;
		default:
			ui->stackedWidget->setCurrentIndex(0);
	}
}

bool MainWindow::SaveOrder(FrontsDataStruct::Order order, bool saveAS)
{
	if(_fileData->SaveOrder(order, saveAS) == false)
		return false;
	_settings->setValue(QVariant::fromValue(SETTINGS_KEYS::LAST_SAVE_PATH).toString(),_fileData->GetCurrenPath());

	this->setWindowTitle(_windowTitle + " - " + _fileData->GetFileName());
	return true;
}

void MainWindow::createMenus()
{

}

void MainWindow::loadOrder(QString path)
{
	FrontsDataStruct::Order loadedOrder;
	bool flag = true;
	try
	{
		loadedOrder = _fileData->LoadOrder(path);

	} catch (QString e)
	{
		flag = false;
	}
	if(flag)
	{
		_pageOrder->Clear();
		_pageOrder->Load(loadedOrder);
		this->setWindowTitle(_windowTitle + " - " + _fileData->GetFileName());
		_settings->setValue(QVariant::fromValue(SETTINGS_KEYS::LAST_OPEN_PATH).toString(),_fileData->GetCurrenPath());
		ChangePage(PAGES::ORDER);
	}
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if(_pageOrder->IsOrderUpdated())
	{
		QMessageBox msg(QMessageBox::Warning,"Zmodyfikowano zamównienie", "Zamówienie zostało zmodyfikowane. Zapisać teraz?",QMessageBox::Yes | QMessageBox::No);
		if(msg.exec() == QMessageBox::Yes)
			_pageOrder->ActionSave();
	}

	if(this->isMaximized() == false)
	{
		_settings->setValue("WindowSize",this->size());
		_settings->setValue("WindowPosition",this->pos());
	}
	_settings->setValue("WindowIsMaximized",this->isMaximized());
	event->accept();
}

void MainWindow::showEvent(QShowEvent *event)
{
	if(!_settings->value("WindowPosition").isNull())
		this->move(_settings->value("WindowPosition").toPoint());
	if(!_settings->value("WindowSize").isNull())
		this->resize(_settings->value("WindowSize").toSize());
	if(_settings->value("WindowIsMaximized",false).toBool())
		this->setWindowState(Qt::WindowState::WindowMaximized);
	event->accept();
}

void MainWindow::on_actionNew_triggered()
{
	_pageOrder->Clear();
	_fileData->Clear();
	this->setWindowTitle(_windowTitle);
	ChangePage(PAGES::ORDER);
}

void MainWindow::on_actionOpen_triggered()
{
	loadOrder();

}

void MainWindow::on_actionFont_triggered()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok,this);
	if(ok)
	{
		QApplication::setFont(font);
		_settings->setValue("Font", font.toString());
	}

}

void MainWindow::on_actionBuyer_triggered()
{

}

void MainWindow::on_actionSaveAs_triggered()
{
	_pageOrder->ActionSaveAs();
}

void MainWindow::on_actionSave_triggered()
{
	_pageOrder->ActionSave();
}

void MainWindow::on_actionClose_triggered()
{
	close();
}

void MainWindow::on_actionPrint_triggered()
{
	_pageOrder->ActionPrint();
}
