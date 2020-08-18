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

	ui->setupUi(this);

	ui->actionConnect->setEnabled(true);
	ui->actionDisconnect->setEnabled(false);
	ui->actionFind->setEnabled(false);
	ui->actionImport->setEnabled(false);

	Worker = new ImportWorker(Database, this);
	Worker->moveToThread(&Thread);
	Thread.start();

	connect(ui->actionConnect, &QAction::triggered,
		   this, &MainWindow::connectActionClicked);

	connect(ui->actionImport, &QAction::triggered,
		   this, &MainWindow::importActionClicked);
}

MainWindow::~MainWindow(void)
{
	Thread.exit();
	Thread.wait();

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

	connect(Dialog, &ImportDialog::onAccept, this, &MainWindow::importDocuments);
	connect(Dialog, &ImportDialog::onAccept, Worker, &ImportWorker::importDocuments);
	connect(Dialog, &ImportDialog::accepted, Dialog, &ImportDialog::deleteLater);
	connect(Dialog, &ImportDialog::rejected, Dialog, &ImportDialog::deleteLater);
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
}

void MainWindow::importDocuments(void)
{
	// TODO: lock actions and parts of UI
}

