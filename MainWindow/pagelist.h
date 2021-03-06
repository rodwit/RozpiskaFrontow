#ifndef PAGELIST_H
#define PAGELIST_H

#include <QWidget>

#include "dialogfront.h"

namespace Ui {
	class PageList;
}

class MainWindow;

class PageList : public QWidget
{
		Q_OBJECT

	public:
		explicit PageList(QWidget *parent = nullptr);
		~PageList();
		void Clear();
		FrontsDataStruct::FrontsTypeList GetFrontsList();
		void LoadFronts(FrontsDataStruct::FrontsTypeList frontsList);

	public slots:
		void ActionAdd() {on_pushButtonAdd_clicked();};
		void ActionDelete() {on_pushButtonDelete_clicked();};
		void ActionInsert() {on_pushButtonInsert_clicked();};
		void ActionEdit();

	private slots:
		void on_pushButtonAdd_clicked();

		void on_pushButtonInsert_clicked();

		void updateCounter();

		void on_pushButtonDelete_clicked();

		void on_tableWidgetFronts_cellDoubleClicked(int row, int column);

		void on_buttonBox_accepted();
		void on_buttonBox_rejected();

	signals:
		void ListResult(bool);

	private:
		Ui::PageList *ui;
		MainWindow* getParent() const;

		/*!
		 * \fn changefrontsNumber
		 * \obsolete obsolete
		 */
		void changefrontsNumber(int value, char sign);

		/*!
		 * \param front
		 * \return true if front ok
		 */
		bool checkIfDuplicated(FrontsDataStruct::Front front,int updatedRow = -1);

		void addFront(FrontsDataStruct::Front front, int insert = -1);


		// QWidget interface
	protected:
		void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // PAGELIST_H
