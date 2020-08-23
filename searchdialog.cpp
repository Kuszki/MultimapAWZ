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

#include "searchdialog.hpp"
#include "ui_searchdialog.h"

SearchDialog::SearchDialog(QSqlDatabase& Db, QWidget *parent)
: QDialog(parent), Database(Db), ui(new Ui::SearchDialog)
{
	ui->setupUi(this); validateData();

	connect(ui->buttonBox->button(QDialogButtonBox::Reset),
		   &QPushButton::clicked, this, &SearchDialog::onReset);

	connect(ui->buttonBox->button(QDialogButtonBox::Apply),
		   &QPushButton::clicked, this, &SearchDialog::accept);

	connect(ui->buttonBox->button(QDialogButtonBox::Reset),
		   &QPushButton::clicked, this, &SearchDialog::close);
}

SearchDialog::~SearchDialog(void)
{
	delete ui;
}

void SearchDialog::open(void)
{
	const QString lastComm = ui->commEdit->currentText();
	const QString lastPrec = ui->precEdit->currentText();

	ui->commEdit->clear();
	ui->precEdit->clear();

	QSqlQuery Query(Database);
	Query.prepare("SELECT id, nazwa FROM gminy ORDER BY nazwa ASC");

	if (Query.exec()) while (Query.next())
	{
		ui->commEdit->addItem(Query.value(1).toString(), Query.value(0));
	}

	ui->commEdit->setCurrentText(lastComm);
	ui->precEdit->setCurrentText(lastPrec);

	QDialog::open();
}

void SearchDialog::accept(void)
{
	QDialog::accept(); QVariantMap Map;

	if (ui->awzEdit->isEnabled()) Map.insert("awz", ui->awzEdit->text().simplified());
	if (ui->lotEdit->isEnabled()) Map.insert("lot", ui->lotEdit->text().simplified());
	if (ui->nameEdit->isEnabled()) Map.insert("name", ui->nameEdit->text().simplified());
	if (ui->commEdit->isEnabled()) Map.insert("comm", ui->commEdit->currentData());
	if (ui->precEdit->isEnabled()) Map.insert("prec", ui->precEdit->currentData());

	emit onAccept(Map);
}

void SearchDialog::commIndexChanged(int Index)
{
	if (Index < 0) return;
	else ui->precEdit->clear();

	QSqlQuery Query(Database);
	Query.prepare("SELECT id, nazwa FROM obreby WHERE id_gminy = ? ORDER BY nazwa ASC");
	Query.addBindValue(ui->commEdit->currentData());

	if (Query.exec()) while (Query.next())
	{
		ui->precEdit->addItem(Query.value(1).toString(), Query.value(0));
	}
}

void SearchDialog::precCheckToggled(bool Enable)
{
	if (Enable) ui->commCheck->setEnabled(true);
}

void SearchDialog::commCheckToggled(bool Enable)
{
	if (!Enable) ui->precCheck->setEnabled(false);
}

void SearchDialog::validateData(void)
{
	const bool OK = ui->awzCheck->isChecked() ||
				 ui->lotCheck->isChecked() ||
				 ui->nameCheck->isChecked() ||
				 ui->commCheck->isChecked() ||
				 ui->commCheck->isChecked();

	ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(OK &&
		(!ui->awzEdit->isEnabled() || !ui->awzEdit->text().simplified().isEmpty()) &&
		(!ui->lotEdit->isEnabled() || !ui->lotEdit->text().simplified().isEmpty()) &&
		(!ui->nameEdit->isEnabled() || !ui->nameEdit->text().simplified().isEmpty()));
}
