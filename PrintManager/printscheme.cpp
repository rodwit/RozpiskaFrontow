#include "printscheme.h"
#include "ui_printscheme.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QDate>

PrintScheme::PrintScheme(FrontsDataStruct::Order order,QString buyerAddress, int fontSize, int typeSize, int tableSize,
						 int descriptionSize, QWidget *parent) :
	QWidget(parent),_order(order), _fontSize(fontSize), _typeSize(typeSize),
	_tableSize(tableSize), _descSize(descriptionSize), ui(new Ui::PrintScheme)
{
	ui->setupUi(this);

	this->setContentsMargins(50,0,200,50);

	_mainLayout = new QFormLayout(this);
	this->setStyleSheet("background-color: white;");

	header(buyerAddress);

	createTables();

	setLayout(_mainLayout);
}

PrintScheme::~PrintScheme()
{
	delete ui;
}

void PrintScheme::header(QString address)
{
	QLabel *label = new QLabel;
	label->setTextFormat(Qt::TextFormat::RichText);
	label->setText("<p style=\"font-size:" +QString::number(_fontSize) +"px\">" + address + "<br>"+_order.sprayer + "<br>Zamówienie dn. " + QDate::currentDate().toString("dd MMMM yyyy") +"<br>( <b>" + _order.name + "</b> )</p>" );
	label->setAlignment(Qt::AlignmentFlag::AlignCenter);
	_mainLayout->addRow(label);

}

void PrintScheme::createTables()
{
	for (auto type : _order.list)
	{
		int typeSize = (_typeSize <=0) ? _fontSize : _typeSize;
		int tableSize = (_tableSize <=0) ? _fontSize : _tableSize;
		int descSize = (_descSize<=0) ? _fontSize : _descSize;
		QLabel *labelFronts = new QLabel;
		QLabel *labelSummary = new QLabel;
		QHBoxLayout *hLayout = new QHBoxLayout;
		labelFronts->setTextFormat(Qt::TextFormat::RichText);
		labelSummary->setTextFormat(Qt::TextFormat::RichText);

		QString stringFronts;
		QString stringSummary;
		stringSummary += "<style>.Summary {font-size:"+QString::number(_fontSize)+"px;}</style>";
		stringFronts += "<style>.Type {font-size:"+QString::number(typeSize)+"px; font-weight: bold;}\
						.Table {font-size:"+QString::number(tableSize)+"px;}\
						.colorCell{background-color: #FFFF00;}\
						td{padding-bottom: 4px; padding-top: 4px; vertical-align:middle;}\
						.Desc {font-size:"+QString::number(descSize)+"px; text-align:center;}</style>";

		stringFronts += "<div class=\"Type\">" + type.type + "</div></br>";
		stringFronts += "<table class=\"Table\"  border=\"0\" width=100% align=\"center\">";
		int fronts = 0;
		float m2 = 0;
		bool colorFlag = true;
		const QString colorCell = "class=\"colorCell\"";
		for(auto ele : type.list)
		{
			colorFlag = !colorFlag;
			QString colorClassString = "";
			if(colorFlag)
				colorClassString = colorCell;
			stringFronts += "<tr>";
			stringFronts += "<td "+colorClassString+">" + QString::number(ele.height) + "</td>";
			if(type.attributes.value(type.UNDERLINE_BOOL).toBool())
				stringFronts += "<td "+colorClassString+"><u>" + QString::number(ele.width) + "</u></td>";
			else
				stringFronts += "<td "+colorClassString+">" + QString::number(ele.width) + "</td>";
			stringFronts += "<td "+colorClassString+">" + QString::number(ele.amount) + "</td>";
			//if description empty then no color
			if(ele.description.isEmpty() || colorFlag == false)
				colorClassString = "class=\"Desc\"";
			else
				colorClassString = "class=\"Desc colorCell\"";
			stringFronts += "<td "+colorClassString+">" + ele.description + " </td>";
			stringFronts += "</tr>";

			fronts += ele.amount; // add front
			m2 += ( (ele.height/1000.0f) * (ele.width/1000.0f) ) * ele.amount;
		}
		stringFronts += "</table>";

		stringSummary += "<div class=\"Summary\">";
		if(type.attributes[type.AMOUNT_BOOL].toBool() == true)
			stringSummary += "Ilość frontów: " + QString::number(fronts) + "<br>";
		if(type.attributes[type.M2_BOOL].toBool() == true)
			stringSummary += "Razem m2: " + QString::number(m2,'g',3);
		stringSummary += "</div>";

		labelFronts->setText(stringFronts);
		labelSummary->setText(stringSummary);
		labelSummary->setAlignment(Qt::AlignTop);

		hLayout->addWidget(labelFronts);
		hLayout->addWidget(labelSummary);

		hLayout->setStretch(0,1);
		hLayout->setSpacing(50);
		_mainLayout->addRow(hLayout);

	}
}
