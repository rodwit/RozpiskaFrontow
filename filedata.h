#ifndef FILEDATAXML_H
#define FILEDATAXML_H

#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include "FrontsDataStruct.h"

class FileData
{
	private:
		QWidget *_parent;
		//int _mode = 0; // -1 load, 1 save
		QFile *_file;
		QString _fileString;
		QDataStream _stream;
		QString _lastSavePath;
		QString _lastOpenPath;
		bool openFile(bool toSave, bool saveAs = false, QString pathArg = "");
	public:
		FileData(QWidget *parent, QString lastSavePath, QString lastOpenPath);
		~FileData();
		bool SaveOrder(FrontsDataStruct::Order order, bool saveAs = false);
		FrontsDataStruct::Order LoadOrder(QString path="");
		QString GetCurrenPath() {return QFileInfo(*_file).path();};
		QString GetFileName() {return QFileInfo(*_file).baseName();};
		void Clear();
};

#endif // FILEDATAXML_H
