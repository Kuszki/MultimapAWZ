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

#include "filewidget.hpp"
#include "ui_filewidget.h"

const QString FileWidget::filterStr = "PLIKI.ID IN (SELECT J.ID_PLI FROM DOK_PLIKI J WHERE J.ID_DOK = %1)";

FileWidget::FileWidget(QSqlDatabase& Db, QWidget *parent)
: QWidget(parent), Database(Db), ui(new Ui::FileWidget)
{
	ui->setupUi(this);

	filter = new ModelFilter(this);
	filter->setSearchedColumns({ 1, 2, 3 });

	ui->tableView->model()->deleteLater();
	ui->tableView->setModel(filter);

	connect(ui->searchEdit, &QLineEdit::textChanged,
		   filter, &ModelFilter::setFilterFixedString);

	connect(ui->tableView->selectionModel(),
		   &QItemSelectionModel::currentRowChanged,
		   this, &FileWidget::rowSelected);
}

FileWidget::~FileWidget(void)
{
	delete ui;
}

void FileWidget::setTitleWidget(TitleWidget* W)
{
	ui->horizontalLayout->removeWidget(ui->reloadButton);
	W->addRightWidget(ui->reloadButton);
}

void FileWidget::filterList(int ID)
{
	if (model) model->setFilter(filterStr.arg(ID));
	if (model && !model->rowCount()) model->select();
}

void FileWidget::reloadList(void)
{
	if (model) model->select();
}

void FileWidget::setStatus(bool Enabled)
{
	setEnabled(Enabled);

	if (Enabled)
	{
		model = new QSqlRelationalTableModel(this, Database);

		model->setTable("PLIKI");

		model->setJoinMode(QSqlRelationalTableModel::LeftJoin);
		model->setRelation(2, QSqlRelation("RODZAJEDOK", "ID", "NAZWA"));

		model->setHeaderData(0, Qt::Horizontal, tr("ID"));
		model->setHeaderData(1, Qt::Horizontal, tr("Name"));
		model->setHeaderData(2, Qt::Horizontal, tr("Role"));
		model->setHeaderData(3, Qt::Horizontal, tr("Comments"));

		model->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);

		filter->setSourceModel(model);

		ui->tableView->setItemDelegate(new ModelDelegate(ui->tableView));
		ui->tableView->hideColumn(0);
	}
	else if (model)
	{
		filter->setSourceModel(nullptr);

		model->deleteLater();
		model = nullptr;
	}
}

void FileWidget::rowSelected(const QModelIndex& Index)
{
	if (!model) return;

	const auto i = ui->tableView->model()->index(Index.row(), 0, Index.parent());
	const int id = ui->tableView->model()->data(i).toInt();

	emit onIndexChange(id);
}
