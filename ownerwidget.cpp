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

#include "ownerwidget.hpp"
#include "ui_ownerwidget.h"

const QString OwnerWidget::filterStr = "OSOBY.ID IN (SELECT J.ID_OSO FROM DOK_OSOBY J WHERE J.ID_DOK = %1)";

OwnerWidget::OwnerWidget(QSqlDatabase& Db, QWidget *parent)
: QWidget(parent), Database(Db), ui(new Ui::OwnerWidget)
{
	ui->setupUi(this);

	filter = new ModelFilter(this);
	filter->setSearchedColumns({ 1, 2 });
	filter->setReadonlyColumns({ 0 });

	ui->tableView->model()->deleteLater();
	ui->tableView->setModel(filter);

	connect(ui->searchEdit, &QLineEdit::textChanged,
		   filter, &ModelFilter::setFilterFixedString);

	connect(ui->tableView->selectionModel(),
		   &QItemSelectionModel::currentRowChanged,
		   this, &OwnerWidget::rowSelected);
}

OwnerWidget::~OwnerWidget(void)
{
	delete ui;
}

void OwnerWidget::setTitleWidget(TitleWidget* W)
{
	ui->horizontalLayout->removeWidget(ui->reloadButton);
	W->addRightWidget(ui->reloadButton);

	ui->horizontalLayout->removeWidget(ui->editButton);
	W->addRightWidget(ui->editButton);
}

void OwnerWidget::filterList(int ID)
{
	if (model) model->setFilter(filterStr.arg(ID));
	if (model && !model->rowCount()) model->select();
}

void OwnerWidget::reloadList(void)
{
	if (model) model->select();
}

void OwnerWidget::setStatus(bool Enabled)
{
	ui->tableView->setEnabled(Enabled);
	ui->editButton->setEnabled(Enabled);
	ui->reloadButton->setEnabled(Enabled);
	ui->searchEdit->setEnabled(Enabled);

	if (Enabled)
	{
		model = new QSqlTableModel(this, Database);

		model->setTable("OSOBY");

		model->setHeaderData(0, Qt::Horizontal, tr("ID"));
		model->setHeaderData(1, Qt::Horizontal, tr("Name"));
		model->setHeaderData(2, Qt::Horizontal, tr("Surname"));
		model->setHeaderData(3, Qt::Horizontal, tr("Father name"));
		model->setHeaderData(4, Qt::Horizontal, tr("Mother name"));

		model->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);

		filter->setSourceModel(model);

		ui->tableView->hideColumn(0);
	}
	else if (model)
	{
		filter->setSourceModel(nullptr);

		model->deleteLater();
		model = nullptr;
	}
}

void OwnerWidget::rowSelected(const QModelIndex& Index)
{
	if (!model) return;

	const auto i = ui->tableView->model()->index(Index.row(), 0, Index.parent());
	const int id = ui->tableView->model()->data(i).toInt();

	emit onIndexChange(id);
}

void OwnerWidget::editClicked(void)
{

}
