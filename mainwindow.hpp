/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  {description}                                                          *
 *  Copyright (C) 2020  Łukasz "Kuszki" Dróżdż  lukasz.kuszki@gmail.com    *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the  Free Software Foundation, either  version 3 of the  License, or   *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This  program  is  distributed  in the hope  that it will be useful,   *
 *  but WITHOUT ANY  WARRANTY;  without  even  the  implied  warranty of   *
 *  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the   *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have  received a copy  of the  GNU General Public License   *
 *  along with this program. If not, see http://www.gnu.org/licenses/.     *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtWidgets>
#include <QtCore>
#include <QtSql>

#include "connectdialog.hpp"
#include "importdialog.hpp"
#include "importworker.hpp"
#include "progressdialog.hpp"
#include "awzwidget.hpp"
#include "filewidget.hpp"
#include "ownerwidget.hpp"
#include "titlewidget.hpp"
#include "lotwidget.hpp"
#include "scanwidget.hpp"
#include "searchdialog.hpp"
#include "summarywidget.hpp"
#include "settingsdialog.hpp"

#include "commonh.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{

		Q_OBJECT

	private:

		Ui::MainWindow *ui;

		ImportWorker* Worker;

		SearchDialog* Search;

		QDockWidget* awzd;
		AwzWidget* awzw;

		QDockWidget* filed;
		FileWidget* filew;

		QDockWidget* ownd;
		OwnerWidget* ownw;

		QDockWidget* lotd;
		LotWidget* lotw;

		QDockWidget* sumd;
		SummaryWidget* sumw;

		QSqlDatabase Database;
		QThread Thread;

		QString treePath;
		int docIndex = 0;

	public:

		explicit MainWindow(QWidget* parent = nullptr);
		virtual ~MainWindow(void) override;

	protected:

		QString getDbRole(void);
		bool tryConnect(const QString& Role);

	private slots:

		void connectActionClicked(void);
		void importActionClicked(void);
		void settingsActionClicked(void);
		void helpActionClicked(void);

		void openDatabase(const QString& Server, const QString& Base,
					   const QString& User, const QString& Pass,
					   const QString& Scanpath);

		void closeDatabase(void);

		void applySettings(const QVariantMap& Map);

		void applyFilter(const QVariantMap& Map);
		void resetFilter(void);

		void renameFile(const QString& Old,
					 const QString& Name);

		void startJob(void);
		void endJob(void);

		void addDoc(const DOKUMENTY& Data);
		void remDoc(int ID);

		void addLot(const DZIALKI& Data);
		void remLot(int ID);

		void addOwn(const OSOBY& Data);
		void remOwn(int ID);

		void addFil(const PLIKI& Data);
		void remFil(int ID);

	signals:

		void onLogin(bool);
		void onError(const QString&);

};
#endif // MAINWINDOW_HPP
