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

	ui->documentButton->setFixedSize(ui->documentEdit->sizeHint().height(),
							   ui->documentEdit->sizeHint().height());
	ui->fileButton->setFixedSize(ui->fileEdit->sizeHint().height(),
						    ui->fileEdit->sizeHint().height());
	ui->dictButton->setFixedSize(ui->dictEdit->sizeHint().height(),
						    ui->dictEdit->sizeHint().height());
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
						 QString(';') :
						 ui->fseparatorEdit->text();

	const QString lSep = ui->lseparatorEdit->text().isEmpty() ?
						 QString(',') :
						 ui->lseparatorEdit->text();

	switch (ui->tabWidget->currentIndex())
	{
		case 0:
		{
			Roles.insert(ROLES::NUMER, ui->docNameSpin->value() - 1);
			Roles.insert(ROLES::OZNACZENIE, ui->docMarkSpin->value() - 1);
			Roles.insert(ROLES::GMINA, ui->docCommunitySpin->value() - 1);
			Roles.insert(ROLES::OBREB, ui->docPrecinctSpin->value() - 1);
			Roles.insert(ROLES::DZIALKI, ui->docLootSpin->value() - 1);
			Roles.insert(ROLES::OSOBY, ui->docOwnerSpin->value() - 1);
			Roles.insert(ROLES::UWAGI, ui->docCommentSpin->value() - 1);

			emit onDocAccept(ui->documentEdit->text(), Roles, fSep, lSep);
		}
		break;
		case 1:
		{
			Roles.insert(ROLES::NUMER, ui->fileNameSpin->value() - 1);
			Roles.insert(ROLES::PLIK, ui->filePathSpin->value() - 1);
			Roles.insert(ROLES::ROLA, ui->fileRoleSpin->value() - 1);
			Roles.insert(ROLES::UWAGI, ui->fileCommentSpin->value() - 1);

			emit onFileAccept(ui->fileEdit->text(), Roles, fSep);
		}
		break;
		case 2:
		{
			Roles.insert(ROLES::ID, ui->dictIdSpin->value() - 1);
			Roles.insert(ROLES::NUMER, ui->dictNameSpin->value() - 1);

			emit onDictAccept(ui->dictEdit->text(), Roles, fSep);
		}
		break;
	}
}

void ImportDialog::checkData(void)
{
	bool OK = true;

	switch (ui->tabWidget->currentIndex())
	{
		case 0:
		{
			QSet<int> Roles; OK = !ui->documentEdit->text().isEmpty();

			const QString fSep = ui->fseparatorEdit->text().isEmpty() ?
								 QString(';') :
								 ui->fseparatorEdit->text();

			const QString lSep = ui->lseparatorEdit->text().isEmpty() ?
								 QString(',') :
								 ui->lseparatorEdit->text();

			Roles.insert(ui->docNameSpin->value());
			Roles.insert(ui->docMarkSpin->value());
			Roles.insert(ui->docCommunitySpin->value());
			Roles.insert(ui->docPrecinctSpin->value());
			Roles.insert(ui->docLootSpin->value());
			Roles.insert(ui->docOwnerSpin->value());
			Roles.insert(ui->docCommentSpin->value());

			OK = OK && (Roles.size() == 7) && (fSep != lSep);

			for (const auto& i : Roles)
				OK = OK && (i <= ui->docOwnerSpin->value());
		}
		break;
		case 1:
		{
			QSet<int> Roles; OK = !ui->fileEdit->text().isEmpty();

			Roles.insert(ui->fileNameSpin->value());
			Roles.insert(ui->filePathSpin->value());
			Roles.insert(ui->fileRoleSpin->value());
			Roles.insert(ui->fileCommentSpin->value());

			OK = OK && (Roles.size() == 4);
		}
		break;
		case 2:
		{
			QSet<int> Roles; OK = !ui->dictEdit->text().isEmpty();

			Roles.insert(ui->dictIdSpin->value());
			Roles.insert(ui->dictNameSpin->value());

			OK = OK && (Roles.size() == 2);
		}
		break;
	}

	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(OK);
}

void ImportDialog::openDocument(void)
{
	const QString Path = QFileDialog::getOpenFileName(this, tr("Open documents list"), QString(),
											tr("Text files (*.txt);;CSV files (*.csv)"));

	if (!Path.isEmpty()) ui->documentEdit->setText(Path);
}

void ImportDialog::openFile(void)
{
	const QString Path = QFileDialog::getOpenFileName(this, tr("Open files list"), QString(),
											tr("Text files (*.txt);;CSV files (*.csv)"));

	if (!Path.isEmpty()) ui->fileEdit->setText(Path);
}

void ImportDialog::openDict(void)
{
	const QString Path = QFileDialog::getOpenFileName(this, tr("Open dictionary list"), QString(),
											tr("Text files (*.txt);;CSV files (*.csv)"));

	if (!Path.isEmpty()) ui->dictEdit->setText(Path);
}
