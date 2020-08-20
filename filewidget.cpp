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
#include <QDebug>
#include "filewidget.hpp"
#include "ui_filewidget.h"

const QString FileWidget::filterStr = "PLIKI.ID IN (SELECT J.ID_PLI FROM DOK_PLIKI J WHERE J.ID_DOK = %1)";

FileWidget::FileWidget(QSqlDatabase& Db, QWidget *parent)
: QWidget(parent), Database(Db), ui(new Ui::FileWidget)
{
	ui->setupUi(this);

	filter = new ModelFilter(this);
	filter->setSearchedColumns({ 2, 3, 4 });
	filter->setReadonlyColumns({ 0, 1 });

	ui->tableView->model()->deleteLater();
	ui->tableView->setModel(filter);

	connect(filter, &ModelFilter::onRecordUpdate,
		   this, &FileWidget::rowUpdated);

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

	ui->horizontalLayout->removeWidget(ui->editButton);
	W->addRightWidget(ui->editButton);
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
	ui->tableView->setEnabled(Enabled);
	ui->editButton->setEnabled(Enabled);
	ui->reloadButton->setEnabled(Enabled);
	ui->searchEdit->setEnabled(Enabled);

	if (Enabled)
	{
		model = new QSqlRelationalTableModel(this, Database);

		model->setTable("PLIKI");

		model->setJoinMode(QSqlRelationalTableModel::LeftJoin);
		model->setRelation(3, QSqlRelation("RODZAJEDOK", "ID", "NAZWA"));

		model->setHeaderData(0, Qt::Horizontal, tr("ID"));
		model->setHeaderData(1, Qt::Horizontal, tr("Path"));
		model->setHeaderData(2, Qt::Horizontal, tr("Name"));
		model->setHeaderData(3, Qt::Horizontal, tr("Role"));
		model->setHeaderData(4, Qt::Horizontal, tr("Comments"));

		model->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);

		filter->setSourceModel(model);

		ui->tableView->setItemDelegate(new ModelDelegate(ui->tableView));
		ui->tableView->hideColumn(0);
		ui->tableView->hideColumn(1);
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

	const auto i = filter->index(Index.row(), 0, Index.parent());
	const auto p = filter->index(Index.row(), 1, Index.parent());
	const auto f = filter->index(Index.row(), 2, Index.parent());

	qDebug() << i.data().toInt() << p.data().toString() + "/" + f.data().toString();

	emit onIndexChange(i.data().toInt());
	emit onFilepathChange(p.data().toString() + "/" + f.data().toString());
}

void FileWidget::rowUpdated(const QModelIndex& Index, const QVariant& Old, const QVariant& New)
{
	if (Index.column() != 2) return;

	const auto i = filter->index(Index.row(), 1, Index.parent());
	const QString p = filter->data(i).toString();

	emit onFileRename(p + '/' + Old.toString(), New.toString());
}

void FileWidget::editClicked(void)
{
	// TODO implement me
}
