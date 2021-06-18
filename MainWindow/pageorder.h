#ifndef PAGEORDER_H
#define PAGEORDER_H

#include <QWidget>
#include <QListWidget>
#include "FrontsDataStruct.h"

namespace Ui {
	class PageOrder;
}
class MainWindow;
class PageList;
class PageOrder : public QWidget
{
		Q_OBJECT

	public:
		explicit PageOrder(QWidget *parent, PageList *pageList);
		~PageOrder();
		void Load(FrontsDataStruct::Order order);
		void Clear();



	public slots:
		void GetListResult(bool);
		void ActionSave() {on_pushButtonSave_clicked();};
		void ActionSaveAs();
		void ActionAdd() {on_pushButtonAdd_clicked();};
		void ActionDelete() {on_pushButtonDelete_clicked();};
		void ActionEdit();
		void ActionPrint() {on_pushButtonPrint_clicked();};
		bool IsOrderUpdated() { return _isOrderUpdated;};

	private slots:
		void on_pushButtonAdd_clicked();

		void on_pushButtonDelete_clicked();

		void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

		void on_pushButtonMove_clicked();

		void on_listWidget_itemClicked(QListWidgetItem *item);

		void on_pushButtonSave_clicked();

		void on_pushButtonPrint_clicked();

		void on_pushButtonClose_clicked();

	private:
		Ui::PageOrder *ui;
		MainWindow *_parent;
		bool _isOrderUpdated;
		PageList *_pageList;
		bool _editList;
		int _transmittedItem;
		FrontsDataStruct::Order _order;
		void getSprayers();
		MainWindow* getParent() const;
		void addFrontsList(FrontsDataStruct::FrontsTypeList list);
		QString _address;

};

#endif // PAGEORDER_H
