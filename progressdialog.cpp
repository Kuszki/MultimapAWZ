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

#include "progressdialog.hpp"
#include "ui_progressdialog.h"

ProgressDialog::ProgressDialog(QWidget *parent)
: QDialog(parent), ui(new Ui::ProgressDialog)
{
	ui->setupUi(this);

	ui->mainProgress->setVisible(false);
	ui->minorProgress->setVisible(false);

	ui->buttonBox->setVisible(false);
}

ProgressDialog::~ProgressDialog(void)
{
	delete ui;
}

void ProgressDialog::accept(void)
{
	if (!isAction) QDialog::accept();
}

void ProgressDialog::reject(void)
{
	if (!isAction) QDialog::reject();
}

void ProgressDialog::open(void)
{
	isAction = true;

	QDialog::open();
}

void ProgressDialog::startAction(const QString& Action)
{
	ui->jobLabel->setText(Action);

	ui->mainProgress->setRange(0, 0);
	ui->mainProgress->setVisible(true);

	ui->minorProgress->setRange(0, 0);
	ui->minorProgress->setVisible(false);
}

void ProgressDialog::startProcess(int a, int b)
{
	ui->mainProgress->setRange(a, b);
	ui->mainProgress->setVisible(true);
}

void ProgressDialog::updateProcess(int n)
{
	ui->mainProgress->setValue(n);
}

void ProgressDialog::startSubprocess(int a, int b)
{
	ui->minorProgress->setRange(a, b);
	ui->minorProgress->setVisible(true);
}

void ProgressDialog::updateSubprocess(int n)
{
	ui->minorProgress->setValue(n);
}

void ProgressDialog::endJob(const QString& Msg)
{
	ui->jobLabel->setText(Msg);

	ui->mainProgress->setRange(0, 0);
	ui->mainProgress->setVisible(false);

	ui->minorProgress->setRange(0, 0);
	ui->minorProgress->setVisible(false);

	ui->buttonBox->setVisible(true);

	isAction = false;
}
