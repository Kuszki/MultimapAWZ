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
#include <QtSql>

#include "commonh.hpp"
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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{

		Q_OBJECT

	private:

		Ui::MainWindow *ui;

		ImportWorker* Worker;

		QDockWidget* awzd;
		AwzWidget* awzw;

		QDockWidget* filed;
		FileWidget* filew;

		QDockWidget* ownd;
		OwnerWidget* ownw;

		QDockWidget* lotd;
		LotWidget* lotw;

		QDockWidget* scand;
		ScanWidget* scanw;

		QSqlDatabase Database;
		QThread Thread;

		QString treePath;

	public:

		explicit MainWindow(QWidget* parent = nullptr);
		virtual ~MainWindow(void) override;

	private slots:

		void connectActionClicked(void);
		void importActionClicked(void);

		void openDatabase(const QString& Server, const QString& Base,
					   const QString& User, const QString& Pass,
					   const QString& Scanpath);

		void closeDatabase(void);

		void renameFile(const QString& Old,
					 const QString& Name);

		void startJob(void);
		void endJob(void);

	signals:

		void onLogin(bool);
		void onError(const QString&);

};
#endif // MAINWINDOW_HPP
