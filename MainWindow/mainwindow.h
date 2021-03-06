#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


#include "pagelist.h"
#include "pageorder.h"
#include "filedata.h"

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		MainWindow(QWidget *parent = nullptr);
		~MainWindow();

		enum PAGES
		{
			MAIN,
			ORDER,
			LIST
		};

		enum SETTINGS_KEYS
		{
			LAST_SAVE_PATH,
			LAST_OPEN_PATH
		};

		Q_ENUM(SETTINGS_KEYS)

		void ChangePage(PAGES page);
		QSettings* GetSettings() {return _settings;}
		void SaveOrder(FrontsDataStruct::Order order, bool saveAS = false);

	private slots:
		void on_actionNew_triggered();

		void on_actionOpen_triggered();

		void on_actionFont_triggered();

		void on_actionBuyer_triggered();

		void on_actionSaveAs_triggered();

		void on_actionSave_triggered();

		void on_actionClose_triggered();

		void on_actionPrint_triggered();

	private:
		Ui::MainWindow *ui;
		PageList *_pageList;
		PageOrder *_pageOrder;
		QSettings *_settings;
		FileData *_fileData;
		const QString _windowTitle = "Rozpiska Frontów";
;
		void createMenus();

		void loadOrder(QString path="");
	protected:
		void closeEvent(QCloseEvent *event) override;
		void showEvent(QShowEvent *event) override;
};
#endif // MAINWINDOW_H
