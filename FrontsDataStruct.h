#ifndef FRONTSDATASTRUCT_H
#define FRONTSDATASTRUCT_H

#include <QString>
#include <QMap>
#include <QVariant>

namespace FrontsDataStruct
{
	struct Front;
	struct FrontsTypeList;
	struct Order;
}

struct FrontsDataStruct::Front
{
	public:
		int height;
		int width;
		int amount;
		QString description;
};

struct FrontsDataStruct::FrontsTypeList
{
	public:
		enum ATTRIBUTES {AMOUNT_BOOL,M2_BOOL,UNDERLINE_BOOL};
		QString type;
		QMap <ATTRIBUTES,QVariant> attributes; //e.g fronts count, underline dimension etc.
		QList <Front> list;
};

struct FrontsDataStruct::Order
{
	public:
		QString name;
		QString sprayer;
		QList <FrontsTypeList> list;
};


#endif // FRONTSDATASTRUCT_H
