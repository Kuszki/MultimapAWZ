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

#include "awzwidget.hpp"
#include "ui_awzwidget.h"

AwzWidget::AwzWidget(QSqlDatabase& Db, QWidget* parent)
: QWidget(parent), Database(Db), ui(new Ui::AwzWidget)
{
	ui->setupUi(this);

	filter = new ModelFilter(this);
	filter->setSearchedColumns({ 1, 2 });

	ui->tableView->model()->deleteLater();
	ui->tableView->setModel(filter);

	connect(ui->searchEdit, &QLineEdit::textChanged,
		   filter, &ModelFilter::setFilterFixedString);

	connect(ui->tableView->selectionModel(),
		   &QItemSelectionModel::currentRowChanged,
		   this, &AwzWidget::rowSelected);
}

AwzWidget::~AwzWidget(void)
{
	delete ui;
}

void AwzWidget::setTitleWidget(TitleWidget* W)
{
	ui->horizontalLayout->removeWidget(ui->reloadButton);
	W->addRightWidget(ui->reloadButton);
}

void AwzWidget::filterList(const QSet<int>& List)
{
	if (model) filter->setFilterIndexes(List);
}

void AwzWidget::reloadList(void)
{
	if (model) model->select();
}

void AwzWidget::setStatus(bool Enabled)
{
	setEnabled(Enabled);

	if (Enabled)
	{
		model = new QSqlTableModel(this, Database);

		model->setTable("DOKUMENTY");

		model->setHeaderData(0, Qt::Horizontal, tr("ID"));
		model->setHeaderData(1, Qt::Horizontal, tr("Name"));
		model->setHeaderData(2, Qt::Horizontal, tr("Comments"));

		model->setEditStrategy(QSqlTableModel::OnFieldChange);

		model->select();

		filter->setSourceModel(model);

		ui->tableView->hideColumn(0);
		ui->tableView->hideColumn(2);
	}
	else if (model)
	{
		filter->setSourceModel(nullptr);

		model->deleteLater();
		model = nullptr;
	}
}

void AwzWidget::rowSelected(const QModelIndex& Index)
{
	if (!model) return;

	const auto i = ui->tableView->model()->index(Index.row(), 0, Index.parent());
	const int id = ui->tableView->model()->data(i).toInt();

	emit onIndexChange(id);
}

