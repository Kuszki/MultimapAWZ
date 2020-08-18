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

#include "importdialog.hpp"
#include "ui_importdialog.h"

ImportDialog::ImportDialog(QWidget *parent)
: QDialog(parent), ui(new Ui::ImportDialog)
{
	ui->setupUi(this); checkData();
}

ImportDialog::~ImportDialog(void)
{
	delete ui;
}

void ImportDialog::accept(void)
{
	QDialog::accept();
	QMap<ROLES, int> Roles;

	const QString fSep = ui->fseparatorEdit->text().isEmpty() ?
						 ui->fseparatorEdit->placeholderText() :
						 ui->fseparatorEdit->text();

	const QString lSep = ui->lseparatorEdit->text().isEmpty() ?
						 ui->lseparatorEdit->placeholderText() :
						 ui->lseparatorEdit->text();

	Roles.insert(ROLES::NUMER, ui->nameSpin->value() - 1);
	Roles.insert(ROLES::GMINA, ui->communitySpin->value() - 1);
	Roles.insert(ROLES::OBREB, ui->precinctSpin->value() - 1);
	Roles.insert(ROLES::DZIALKI, ui->lootSpin->value() - 1);
	Roles.insert(ROLES::OSOBY, ui->ownerSpin->value() - 1);
	Roles.insert(ROLES::UWAGI, ui->commentSpin->value() - 1);

	emit onAccept(ui->documentEdit->text(),
			    ui->fileEdit->text(),
			    ui->dictEdit->text(),
			    Roles, fSep, lSep);
}

void ImportDialog::checkData(void)
{
	QSet<int> Roles; bool OK = true;

	const bool isDoc = !ui->documentEdit->text().isEmpty();
	const bool isFile = !ui->fileEdit->text().isEmpty();

	const QString fSep = ui->fseparatorEdit->text().isEmpty() ?
						 QString(';') :
						 ui->fseparatorEdit->text();

	const QString lSep = ui->lseparatorEdit->text().isEmpty() ?
						 QString(',') :
						 ui->lseparatorEdit->text();

	Roles.insert(ui->nameSpin->value());
	Roles.insert(ui->communitySpin->value());
	Roles.insert(ui->precinctSpin->value());
	Roles.insert(ui->lootSpin->value());
	Roles.insert(ui->ownerSpin->value());
	Roles.insert(ui->commentSpin->value());

	if (isDoc) OK = Roles.size() == 6;
	else OK = !ui->fileEdit->text().isEmpty() ||
			!ui->dictEdit->text().isEmpty();

	if (isDoc && isFile) OK = OK && (fSep != lSep);

	for (const auto& i : Roles)
		OK = OK && (i <= ui->ownerSpin->value());

	ui->nameSpin->setEnabled(isDoc);
	ui->communitySpin->setEnabled(isDoc);
	ui->precinctSpin->setEnabled(isDoc);
	ui->lootSpin->setEnabled(isDoc);
	ui->ownerSpin->setEnabled(isDoc);
	ui->commentSpin->setEnabled(isDoc);
	ui->lseparatorEdit->setEnabled(isDoc);

	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(OK);
}

void ImportDialog::openDocument(void)
{
	const QString Path = QFileDialog::getOpenFileName(this, tr("Open documents list"), QString(),
											tr("Text files (*.txt);CSV files (*.csv)"));

	if (!Path.isEmpty()) ui->documentEdit->setText(Path);
}

void ImportDialog::openFile(void)
{
	const QString Path = QFileDialog::getOpenFileName(this, tr("Open files list"), QString(),
											tr("Text files (*.txt);CSV files (*.csv)"));

	if (!Path.isEmpty()) ui->fileEdit->setText(Path);
}
