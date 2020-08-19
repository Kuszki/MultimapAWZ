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

	ui->actionConnect->setEnabled(true);
	ui->actionDisconnect->setEnabled(false);
	ui->actionFind->setEnabled(false);
	ui->actionImport->setEnabled(false);

	Worker = new ImportWorker(Database);
	Worker->moveToThread(&Thread);
	Thread.start();

	awzw = new AwzWidget(Database, this);
	awzd = new QDockWidget(tr("Documents"), this);
	awzd->setWindowIcon(awzw->windowIcon());
	awzd->setObjectName("Documents");
	awzw->setEnabled(false);
	awzd->setWidget(awzw);

	filew = new FileWidget(Database, this);
	filed = new QDockWidget(tr("Files"), this);
	filed->setWindowIcon(filew->windowIcon());
	filed->setObjectName("Files");
	filew->setEnabled(false);
	filed->setWidget(filew);

	ownw = new OwnerWidget(Database, this);
	ownd = new QDockWidget(tr("Owners"), this);
	ownd->setWindowIcon(ownw->windowIcon());
	ownd->setObjectName("Owners");
	ownw->setEnabled(false);
	ownd->setWidget(ownw);

	awzw->setTitleWidget(new TitleWidget(awzd));
	filew->setTitleWidget(new TitleWidget(filed));
	ownw->setTitleWidget(new TitleWidget(ownd));

	addDockWidget(Qt::LeftDockWidgetArea, awzd);
	addDockWidget(Qt::RightDockWidgetArea, filed);
	addDockWidget(Qt::RightDockWidgetArea, ownd);

	Settings.beginGroup("Window");
	restoreGeometry(Settings.value("geometry").toByteArray());
	restoreState(Settings.value("state").toByteArray());
	Settings.endGroup();

	connect(ui->actionConnect, &QAction::triggered,
		   this, &MainWindow::connectActionClicked);

	connect(ui->actionDisconnect, &QAction::triggered,
		   this, &MainWindow::closeDatabase);

	connect(ui->actionImport, &QAction::triggered,
		   this, &MainWindow::importActionClicked);

	connect(Worker, &ImportWorker::onJobEnd,
		   this, &MainWindow::endJob);

	connect(this, &MainWindow::onLogin,
		   awzw, &AwzWidget::setStatus);

	connect(this, &MainWindow::onLogin,
		   filew, &FileWidget::setStatus);

	connect(this, &MainWindow::onLogin,
		   ownw, &OwnerWidget::setStatus);

	connect(awzw, &AwzWidget::onIndexChange,
		   filew, &FileWidget::filterList);

	connect(awzw, &AwzWidget::onIndexChange,
		   ownw, &OwnerWidget::filterList);
}

MainWindow::~MainWindow(void)
{
	QSettings Settings("Multimap", "AWZ");

	Settings.beginGroup("Window");
	Settings.setValue("state", saveState());
	Settings.setValue("geometry", saveGeometry());
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

void MainWindow::openDatabase(const QString& Server, const QString& Base, const QString& User, const QString& Pass)
{
	static int defaultPort = 0;

	if (!defaultPort) defaultPort = Database.port();
	if (Database.isOpen()) Database.close();

	Database.setHostName(Server.section(':', 0, 0));
	Database.setDatabaseName(Base);
	Database.setUserName(User);
	Database.setPassword(Pass);

	if (Server.contains(':')) Database.setPort(Server.section(':', 1).toInt());
	else Database.setPort(defaultPort);

	if (Database.open()) emit onLogin(true);
	else emit onError(Database.lastError().text());

	ui->actionConnect->setEnabled(!Database.isOpen());
	ui->actionDisconnect->setEnabled(Database.isOpen());
	ui->actionImport->setEnabled(Database.isOpen());

	ui->actionFind->setEnabled(Database.isOpen());
}

void MainWindow::closeDatabase(void)
{
	Database.close();

	ui->actionConnect->setEnabled(true);
	ui->actionDisconnect->setEnabled(false);
	ui->actionFind->setEnabled(false);

	emit onLogin(false);
}

void MainWindow::startJob(void)
{
	setEnabled(false);
}

void MainWindow::endJob(void)
{
	setEnabled(true);
}

