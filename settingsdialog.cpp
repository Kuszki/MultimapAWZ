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

#include "settingsdialog.hpp"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
: QDialog(parent), ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);

	QSettings Settings("Multimap", "AWZ");

	Settings.beginGroup("Documents");
	ui->docCommentCheck->setChecked(Settings.value("Comment", false).toBool());
	Settings.endGroup();

	Settings.beginGroup("Files");
	ui->filePathCheck->setChecked(Settings.value("Path", false).toBool());
	ui->fileRoleCheck->setChecked(Settings.value("Role", true).toBool());
	ui->fileCommentCheck->setChecked(Settings.value("Comment", false).toBool());
	Settings.endGroup();

	Settings.beginGroup("Lots");
	ui->lotSheetCheck->setChecked(Settings.value("Sheet", true).toBool());
	Settings.endGroup();

	Settings.beginGroup("Owners");
	ui->ownFatherCheck->setChecked(Settings.value("Father", false).toBool());
	ui->ownMatherCheck->setChecked(Settings.value("Mother", false).toBool());
	Settings.endGroup();
}

SettingsDialog::~SettingsDialog(void)
{
	QSettings Settings("Multimap", "AWZ");

	Settings.beginGroup("Documents");
	Settings.setValue("Comment", ui->docCommentCheck->isChecked());
	Settings.endGroup();

	Settings.beginGroup("Files");
	Settings.setValue("Path", ui->filePathCheck->isChecked());
	Settings.setValue("Role", ui->fileRoleCheck->isChecked());
	Settings.setValue("Comment", ui->fileCommentCheck->isChecked());
	Settings.endGroup();

	Settings.beginGroup("Lots");
	Settings.setValue("Sheet", ui->lotSheetCheck->isChecked());
	Settings.endGroup();

	Settings.beginGroup("Owners");
	Settings.setValue("Father", ui->ownFatherCheck->isChecked());
	Settings.setValue("Mother", ui->ownMatherCheck->isChecked());
	Settings.endGroup();

	delete ui;
}

void SettingsDialog::accept(void)
{
	QVariantMap Map; QDialog::accept();

	const QVariantMap Doc =
	{
		{ "comm", ui->docCommentCheck->isChecked() }
	};

	const QVariantMap Fil =
	{
		{ "path", ui->filePathCheck->isChecked() },
		{ "role", ui->fileRoleCheck->isChecked() },
		{ "comm", ui->fileCommentCheck->isChecked() }
	};

	const QVariantMap Lot =
	{
		{ "sheet", ui->lotSheetCheck->isChecked() }
	};

	const QVariantMap Own =
	{
		{ "fath", ui->ownFatherCheck->isChecked() },
		{ "moth", ui->ownMatherCheck->isChecked() }
	};

	Map.insert("doc", Doc);
	Map.insert("fil", Fil);
	Map.insert("lot", Lot);
	Map.insert("own", Own);

	emit onAccept(Map);
}
