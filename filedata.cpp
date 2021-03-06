#include "filedata.h"

#include <QUrl>
#include <QFileDialog>
#include <QMessageBox>
#include <QException>

bool FileData::openFile(bool toSave, bool saveAs, QString pathArg)
{
	QString lastPath = (toSave) ? _lastSavePath : _lastOpenPath;

	QString path;
	QFile::OpenMode fileOptions;
	if(toSave)
	{
		if(_fileString.isEmpty() || saveAs)
			path = QFileDialog::getSaveFileName(_parent,"Zapisz plik", lastPath,"Plik RFDAT (*.rfdat)");
		else
			path = _fileString;
		fileOptions = QIODevice::WriteOnly | QIODevice::Truncate;
	}
	else
	{
		if(pathArg == "")
			path = QFileDialog::getOpenFileName(_parent,"Otwórz plik", lastPath,"Plik RFDAT (*.rfdat)");
		else
			path = pathArg;
		fileOptions = QIODevice::ReadOnly;
	}
	if(path.isNull() || path.isEmpty())
		return false;

	_file = new QFile(path);
	if(!_file->open(fileOptions))
	{
		QMessageBox mess(_parent);
		mess.setIcon(QMessageBox::Critical);
		if(toSave)
		{
			mess.setText("Błąd zapisu");
			mess.setInformativeText("Nie udało się zapisać pliku");
		}
		else
		{
			mess.setText("Błąd otwarcia");
			mess.setInformativeText("Nie udało się otworzyć pliku");
		}

		mess.setDefaultButton(QMessageBox::Ok);
		mess.exec();
		return false;
	}

	_stream.setDevice(_file);
	_stream.setVersion(QDataStream::Qt_5_14);
	_fileString = path;

	return true;

}

FileData::FileData(QWidget *parent, QString lastSavePath, QString lastOpenPath)
{
	_lastOpenPath = (lastOpenPath == nullptr) ? QDir::currentPath() : lastOpenPath;
	_lastSavePath = (lastSavePath == nullptr) ? QDir::currentPath() : lastSavePath;
	_parent = parent;
	_file = nullptr;
	_fileString.clear();
}

FileData::~FileData()
{
	if(_file)
		_file->close();

	delete _file;
}


bool FileData::SaveOrder(FrontsDataStruct::Order order, bool saveAs)
{
	bool flag = false;
	if(_file == nullptr)
	{
		if(!openFile(true))
			return false;
		else
			flag = true;
	}

	if(flag == false && saveAs)
		openFile(true,true);

	if(_file->isOpen() == false)
		openFile(true);


	try
	{
		_stream << order.name;
		_stream << order.sprayer;
		for(auto ele : order.list)
		{
			_stream << QString("TYPE");
			_stream << ele.type;
			_stream << ele.attributes;
			for(auto front : ele.list)
			{
				_stream << QString("FRONT");
				_stream << front.height;
				_stream << front.width;
				_stream << front.amount;
				_stream << front.description;
			}
			_stream << QString("END_TYPE");
		}

	}
	catch(QException e)
	{
		_file->close();
		return false;
	}

	_file->flush();
	_file->close();

	return true;
}


FrontsDataStruct::Order FileData::LoadOrder(QString path)
{
	if(path != "")
	{
		if(!openFile(false,false,path))
			throw QString("Load file");
	}
	else if(!openFile(false))
		throw QString("Load file");

	FrontsDataStruct::Order tempOrder;
	_stream >> tempOrder.name;
	_stream >> tempOrder.sprayer;

	FrontsDataStruct::FrontsTypeList *tempTypeList = nullptr;
	while(!_file->atEnd())
	{
		QString type;
		_stream >> type;
		if(type == "TYPE")
		{
			tempTypeList = new FrontsDataStruct::FrontsTypeList();
			_stream >> tempTypeList->type;
			_stream >> tempTypeList->attributes;
		}
		if(type == "FRONT")
		{
			FrontsDataStruct::Front tempFront;
			_stream >> tempFront.height;
			_stream >> tempFront.width;
			_stream >> tempFront.amount;
			_stream >> tempFront.description;
			tempTypeList->list.append(tempFront);
		}
		if(type == "END_TYPE")
		{
			tempOrder.list.append(*tempTypeList);
			delete tempTypeList;
		}
	}

	_file->close();
	return tempOrder;

}

void FileData::Clear()
{
	if(_file && _file->isOpen())
		_file->close();
	delete _file;
	_file = nullptr;
	_fileString.clear();

}
