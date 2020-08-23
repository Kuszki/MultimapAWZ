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

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), ui(new Ui::MainWindow)
{
	Database = QSqlDatabase::addDatabase("QIBASE");

	QSettings Settings("Multimap", "AWZ");

	ui->setupUi(this);

	ui->centralwidget->deleteLater();

	ui->actionConnect->setEnabled(true);
	ui->actionDisconnect->setEnabled(false);
	ui->actionFind->setEnabled(false);
	ui->actionImport->setEnabled(false);

	Worker = new ImportWorker(Database);
	Worker->moveToThread(&Thread);
	Thread.start();

	Search = new SearchDialog(Database, this);

	awzw = new AwzWidget(Database, this);
	awzd = new QDockWidget(tr("Documents"), this);
	awzd->setWindowIcon(awzw->windowIcon());
	awzd->setObjectName("Documents");
	awzw->setStatus(false);
	awzd->setWidget(awzw);

	filew = new FileWidget(Database, this);
	filed = new QDockWidget(tr("Files"), this);
	filed->setWindowIcon(filew->windowIcon());
	filed->setObjectName("Files");
	filew->setStatus(false);
	filed->setWidget(filew);

	ownw = new OwnerWidget(Database, this);
	ownd = new QDockWidget(tr("Owners"), this);
	ownd->setWindowIcon(ownw->windowIcon());
	ownd->setObjectName("Owners");
	ownw->setStatus(false);
	ownd->setWidget(ownw);

	lotw = new LotWidget(Database, this);
	lotd = new QDockWidget(tr("Lots"), this);
	lotd->setWindowIcon(lotw->windowIcon());
	lotd->setObjectName("Lots");
	lotw->setStatus(false);
	lotd->setWidget(lotw);

	scanw = new ScanWidget(treePath, this);
	scand = new QDockWidget(tr("Scans"), this);
	scand->setWindowIcon(scanw->windowIcon());
	scand->setObjectName("Scans");
	scanw->setStatus(false);
	scand->setWidget(scanw);

	awzw->setTitleWidget(new TitleWidget(awzd));
	filew->setTitleWidget(new TitleWidget(filed));
	ownw->setTitleWidget(new TitleWidget(ownd));
	lotw->setTitleWidget(new TitleWidget(lotd));
	scanw->setTitleWidget(new TitleWidget(scand));

	addDockWidget(Qt::LeftDockWidgetArea, awzd);
	addDockWidget(Qt::RightDockWidgetArea, filed);
	addDockWidget(Qt::RightDockWidgetArea, ownd);
	addDockWidget(Qt::RightDockWidgetArea, lotd);
	addDockWidget(Qt::TopDockWidgetArea, scand);

	Settings.beginGroup("Window");
	restoreGeometry(Settings.value("geometry").toByteArray());
	restoreState(Settings.value("state").toByteArray());
	Settings.endGroup();

	if (isMaximized()) setGeometry(QApplication::desktop()->availableGeometry(this));

	Settings.beginGroup("Documents");
	treePath = Settings.value("path").toString();
	Settings.endGroup();

	connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::connectActionClicked);
	connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeDatabase);
	connect(ui->actionFind, &QAction::triggered, Search, &SearchDialog::open);

	connect(ui->actionImport, &QAction::triggered, this, &MainWindow::importActionClicked);

	connect(Worker, &ImportWorker::onJobEnd, this, &MainWindow::endJob);

	connect(Search, &SearchDialog::onAccept, this, &MainWindow::applyFilter);
	connect(Search, &SearchDialog::onReset, this, &MainWindow::resetFilter);

	connect(this, &MainWindow::onLogin, awzw, &AwzWidget::setStatus);
	connect(this, &MainWindow::onLogin, filew, &FileWidget::setStatus);
	connect(this, &MainWindow::onLogin, ownw, &OwnerWidget::setStatus);
	connect(this, &MainWindow::onLogin, lotw, &LotWidget::setStatus);
	connect(this, &MainWindow::onLogin, scanw, &ScanWidget::setStatus);

	connect(awzw, &AwzWidget::onIndexChange, [this] (int ID) { docIndex = ID; } );
	connect(awzw, &AwzWidget::onIndexChange, filew, &FileWidget::filterList);
	connect(awzw, &AwzWidget::onIndexChange, ownw, &OwnerWidget::filterList);
	connect(awzw, &AwzWidget::onIndexChange, lotw, &LotWidget::filterList);

	connect(filew, &FileWidget::onFileRename, this, &MainWindow::renameFile);
	connect(filew, &FileWidget::onFilepathChange, scanw, &ScanWidget::updateImage);

	connect(awzw, &AwzWidget::onAddRow, this, &MainWindow::addDoc);
	connect(awzw, &AwzWidget::onRemRow, this, &MainWindow::remDoc);

	connect(filew, &FileWidget::onAddRow, this, &MainWindow::addFil);
	connect(filew, &FileWidget::onRemRow, this, &MainWindow::remFil);

	connect(lotw, &LotWidget::onAddRow, this, &MainWindow::addLot);
	connect(lotw, &LotWidget::onRemRow, this, &MainWindow::remLot);

	connect(ownw, &OwnerWidget::onAddRow, this, &MainWindow::addOwn);
	connect(ownw, &OwnerWidget::onRemRow, this, &MainWindow::remOwn);
}

MainWindow::~MainWindow(void)
{
	QSettings Settings("Multimap", "AWZ");

	Settings.beginGroup("Window");
	Settings.setValue("state", saveState());
	Settings.setValue("geometry", saveGeometry());
	Settings.endGroup();

	Settings.beginGroup("Documents");
	Settings.setValue("path", treePath);
	Settings.endGroup();

	Thread.exit();
	Thread.wait();

	delete Worker;
	delete ui;
}

void MainWindow::connectActionClicked(void)
{
	ConnectDialog* Dialog = new ConnectDialog(this); Dialog->open();

	connect(Dialog, &ConnectDialog::onAccept, this, &MainWindow::openDatabase);
	connect(Dialog, &ConnectDialog::accepted, Dialog, &ConnectDialog::deleteLater);
	connect(Dialog, &ConnectDialog::rejected, Dialog, &ConnectDialog::deleteLater);

	connect(this, &MainWindow::onLogin, Dialog, &ConnectDialog::connected);
	connect(this, &MainWindow::onError, Dialog, &ConnectDialog::refused);
}

void MainWindow::importActionClicked(void)
{
	ImportDialog* Dialog = new ImportDialog(this); Dialog->open();
	ProgressDialog* Progress = new ProgressDialog(this);

	connect(Dialog, &ImportDialog::accepted, this, &MainWindow::startJob);

	connect(Dialog, &ImportDialog::onDocAccept, Worker, &ImportWorker::importSheets);
	connect(Dialog, &ImportDialog::onFileAccept, Worker, &ImportWorker::importScans);
	connect(Dialog, &ImportDialog::onDictAccept, Worker, &ImportWorker::importDict);

	connect(Dialog, &ImportDialog::accepted, Dialog, &ImportDialog::deleteLater);
	connect(Dialog, &ImportDialog::rejected, Dialog, &ImportDialog::deleteLater);

	connect(Dialog, &ImportDialog::accepted, Progress, &ProgressDialog::open);
	connect(Dialog, &ImportDialog::rejected, Progress, &ProgressDialog::deleteLater);

	connect(Worker, &ImportWorker::onActionStart, Progress, &ProgressDialog::startAction);
	connect(Worker, &ImportWorker::onProgressStart, Progress, &ProgressDialog::startProcess);
	connect(Worker, &ImportWorker::onSubprocessStart, Progress, &ProgressDialog::startSubprocess);
	connect(Worker, &ImportWorker::onProgressUpdate, Progress, &ProgressDialog::updateProcess);
	connect(Worker, &ImportWorker::onSubprocessUpdate, Progress, &ProgressDialog::updateSubprocess);
	connect(Worker, &ImportWorker::onJobEnd, Progress, &ProgressDialog::endJob);

	connect(Progress, &ProgressDialog::accepted, Progress, &ProgressDialog::deleteLater);
	connect(Progress, &ProgressDialog::rejected, Progress, &ProgressDialog::deleteLater);
}

void MainWindow::openDatabase(const QString& Server, const QString& Base, const QString& User, const QString& Pass, const QString& Scanpath)
{
	static int defaultPort = 0;

	if (!Scanpath.isEmpty()) treePath = Scanpath;
	if (!defaultPort) defaultPort = Database.port();
	if (Database.isOpen()) Database.close();

	Database.setHostName(Server.section(':', 0, 0));
	Database.setDatabaseName(Base);
	Database.setUserName(User);
	Database.setPassword(Pass);

	// TODO roles and privileges
	Database.setConnectOptions("ISC_DPB_SQL_ROLE_NAME=ODCZYT");

	if (Server.contains(':')) Database.setPort(Server.section(':', 1).toInt());
	else Database.setPort(defaultPort);

	if (Database.open()) emit onLogin(true);
	else emit onError(Database.lastError().text());

	ui->actionConnect->setEnabled(!Database.isOpen());
	ui->actionDisconnect->setEnabled(Database.isOpen());
	ui->actionImport->setEnabled(Database.isOpen());

	ui->actionFind->setEnabled(Database.isOpen());

	scanw->setPath(Scanpath);
}

void MainWindow::closeDatabase(void)
{
	Database.close();

	ui->actionConnect->setEnabled(true);
	ui->actionDisconnect->setEnabled(false);
	ui->actionFind->setEnabled(false);

	emit onLogin(false);
}

void MainWindow::applyFilter(const QVariantMap& Map)
{
	QSqlQuery Query(Database); QSet<int> List;

	if (Map.contains("awz"))
	{
		QSet<int> Now;

		Query.prepare("SELECT d.id FROM dokumenty d "
				    "WHERE UPPER(d.nazwa) LIKE '%' || UPPER(?) || '%'");
		Query.addBindValue(Map["awz"]);

		if (Query.exec()) while (Query.next())
		{
			Now.insert(Query.value(0).toInt());
		}

		if (List.isEmpty()) List = Now;
		else List = List & Now;
	}

	if (Map.contains("lot"))
	{
		QSet<int> Now;

		Query.prepare("SELECT d.id_dok FROM dok_dzialki d "
				    "INNER JOIN dzialki l ON d.id_dzi = l.id "
				    "WHERE UPPER(l.nazwa) LIKE '%' || UPPER(?) || '%' "
				    "OR UPPER(l.arkusz || '-' || l.nazwa) LIKE '%' || UPPER(?) || '%'");
		Query.addBindValue(Map["lot"]);
		Query.addBindValue(Map["lot"]);

		if (Query.exec()) while (Query.next())
		{
			Now.insert(Query.value(0).toInt());
		}

		if (List.isEmpty()) List = Now;
		else List = List & Now;
	}

	if (Map.contains("name"))
	{
		QSet<int> Now;

		Query.prepare("SELECT d.id_dok FROM dok_osoby d "
				    "INNER JOIN osoby l ON d.id_oso = l.id "
				    "WHERE UPPER(l.nazwisko || ' ' || l.imie) LIKE '%' || UPPER(?) || '%' "
				    "OR UPPER(l.imie || ' ' || l.nazwisko) LIKE '%' || UPPER(?) || '%'");
		Query.addBindValue(Map["name"]);
		Query.addBindValue(Map["name"]);

		if (Query.exec()) while (Query.next())
		{
			Now.insert(Query.value(0).toInt());
		}

		if (List.isEmpty()) List = Now;
		else List = List & Now;
	}

	if (Map.contains("comm"))
	{
		QSet<int> Now;

		Query.prepare("SELECT d.id_dok FROM dok_dzialki d "
				    "INNER JOIN dzialki l ON d.id_dzi = l.id "
				    "INNER JOIN obreby o ON l.id_obrebu = o.id "
				    "WHERE o.id_gminy = ?");
		Query.addBindValue(Map["comm"]);

		if (Query.exec()) while (Query.next())
		{
			Now.insert(Query.value(0).toInt());
		}

		if (List.isEmpty()) List = Now;
		else List = List & Now;
	}

	if (Map.contains("prec"))
	{
		QSet<int> Now;

		Query.prepare("SELECT d.id_dok FROM dok_dzialki d "
				    "INNER JOIN dzialki l ON d.id_dzi = l.id "
				    "INNER JOIN obreby o ON l.id_obrebu = o.id "
				    "WHERE o.id = ?");
		Query.addBindValue(Map["prec"]);

		if (Query.exec()) while (Query.next())
		{
			Now.insert(Query.value(0).toInt());
		}

		if (List.isEmpty()) List = Now;
		else List = List & Now;
	}

	if (List.isEmpty()) awzw->filterList({ -1 });
	else awzw->filterList(List);
}

void MainWindow::resetFilter(void)
{
	awzw->filterList(QSet<int>());
}

void MainWindow::renameFile(const QString& Old, const QString& Name)
{
	QFile::rename(treePath + '/' + Old, Name);
}

void MainWindow::startJob(void)
{
	setEnabled(false);
}

void MainWindow::endJob(void)
{
	setEnabled(true);
}

void MainWindow::addDoc(const DOKUMENTY& Data)
{
	if (Worker->appendDocs({ Data }))
		awzw->reloadList();
}

void MainWindow::remDoc(int ID)
{
	if (Worker->removeDocs({ ID }))
		awzw->reloadList();
}

void MainWindow::addLot(const DZIALKI& Data)
{
	const int cID = Worker->findLots(Data);
	bool OK = false;

	if (cID != -1)
	{
		DOK_DZIALKI rRow = { 0, docIndex, cID };
		OK = Worker->appendDocLot({ rRow  });
	}
	else
	{
		const int fID = Worker->generateId(ImportWorker::Lot);

		DOK_DZIALKI rRow = { 0, docIndex, fID };
		DZIALKI fRow = Data; fRow.id = fID;

		if (Worker->appendLots({ fRow }))
		{
			if (Worker->appendDocLot({ rRow })) OK = true;
			else Worker->removeLots({ fID });
		}
	}

	if (OK) lotw->reloadList();
}

void MainWindow::remLot(int ID)
{
	const bool OK = Worker->removeDocLot(docIndex, ID);

	Worker->removeLots({ ID });

	if (OK) lotw->reloadList();
}

void MainWindow::addOwn(const OSOBY& Data)
{
	const int cID = Worker->findOwns(Data);
	bool OK = false;

	if (cID != -1)
	{
		DOK_OSOBY rRow = { 0, docIndex, cID };
		OK = Worker->appendDocOwn({ rRow  });
	}
	else
	{
		const int fID = Worker->generateId(ImportWorker::Own);

		DOK_OSOBY rRow = { 0, docIndex, fID };
		OSOBY fRow = Data; fRow.id = fID;

		if (Worker->appendOwns({ fRow }))
		{
			if (Worker->appendDocOwn({ rRow })) OK = true;
			else Worker->removeDocs({ fID });
		}
	}

	if (OK) ownw->reloadList();
}

void MainWindow::remOwn(int ID)
{
	const bool OK = Worker->removeDocOwn(docIndex, ID);

	Worker->removeOwns({ ID });

	if (OK) ownw->reloadList();
}

void MainWindow::addFil(const PLIKI& Data)
{
	const int cID = Worker->findFiles(Data);
	bool OK = false;

	if (cID != -1)
	{
		DOK_PLIKI rRow = { 0, docIndex, cID };
		OK = Worker->appendDocFiles({ rRow  });
	}
	else
	{
		const int fID = Worker->generateId(ImportWorker::Fil);

		DOK_PLIKI rRow = { 0, docIndex, fID };
		PLIKI fRow = Data; fRow.id = fID;

		if (Worker->appendFiles({ fRow }))
		{
			if (Worker->appendDocFiles({ rRow })) OK = true;
			else Worker->removeFiles({ fID });
		}
	}

	if (OK) filew->reloadList();
}

void MainWindow::remFil(int ID)
{
	const bool OK = Worker->removeDocFile(docIndex, ID);

	Worker->removeFiles({ ID });

	if (OK) filew->reloadList();
}

