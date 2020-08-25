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
	ui->setupUi(this); setEditable(false);

	hiddenCols = { 0, 1, 3, 4 };

	QSettings Settings("Multimap", "AWZ");

	Settings.beginGroup("Files");
	if (Settings.value("Path", false).toBool()) hiddenCols.remove(1);
	if (Settings.value("Role", true).toBool()) hiddenCols.remove(3);
	if (Settings.value("Comment", false).toBool()) hiddenCols.remove(4);
	Settings.endGroup();

	filter = new ModelFilter(this);
	filter->setSearchedColumns({ 2, 3, 4 });
	filter->setReadonlyColumns({ 0, 1 });

	ui->tableView->model()->deleteLater();
	ui->tableView->setModel(filter);

	ui->reloadButton->setFixedSize(
		ui->searchEdit->sizeHint().height(),
		ui->searchEdit->sizeHint().height());

	connect(filter, &ModelFilter::onRecordUpdate,
		   this, &FileWidget::rowUpdated);

	connect(ui->searchEdit, &QLineEdit::textChanged,
		   filter, &ModelFilter::setFilterFixedString);

	connect(ui->tableView->selectionModel(),
		   &QItemSelectionModel::currentRowChanged,
		   this, &FileWidget::rowSelected);

	connect(ui->tableView->selectionModel(),
		   &QItemSelectionModel::selectionChanged,
		   this, &FileWidget::itemSelected);
}

FileWidget::~FileWidget(void)
{
	delete ui;
}

void FileWidget::setTitleWidget(TitleWidget* W)
{
	ui->horizontalLayout->removeWidget(ui->editButton);
	W->addRightWidget(ui->editButton);

	ui->horizontalLayout->removeWidget(ui->addButton);
	W->addRightWidget(ui->addButton);

	ui->horizontalLayout->removeWidget(ui->remButton);
	W->addRightWidget(ui->remButton);
}

void FileWidget::filterList(int ID)
{
	ui->addButton->setEnabled(ID != -1);

	ui->editButton->setEnabled(false);
	ui->remButton->setEnabled(false);

	if (model) model->setFilter(filterStr.arg(ID));
	if (model && !model->rowCount()) model->select();
}

void FileWidget::reloadList(void)
{
	ui->editButton->setEnabled(false);
	ui->remButton->setEnabled(false);

	if (model) model->select();

	emit onIndexChange(-1);
}

void FileWidget::setStatus(bool Enabled)
{
	ui->tableView->setEnabled(Enabled);
	ui->reloadButton->setEnabled(Enabled);
	ui->searchEdit->setEnabled(Enabled);

	ui->editButton->setEnabled(false);
	ui->addButton->setEnabled(false);
	ui->remButton->setEnabled(false);

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
		model->setHeaderData(4, Qt::Horizontal, tr("Comment"));

		model->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);

		filter->setSourceModel(model);

		ui->tableView->setItemDelegate(new ModelDelegate(ui->tableView));

		for (const auto& i : hiddenCols) ui->tableView->hideColumn(i);
	}
	else if (model)
	{
		filter->setSourceModel(nullptr);

		model->deleteLater();
		model = nullptr;
	}
}

void FileWidget::setEditable(bool Enabled)
{
	ui->addButton->setVisible(Enabled);
	ui->remButton->setVisible(Enabled);
	ui->editButton->setVisible(Enabled);

	ui->tableView->setEditTriggers(Enabled ? QTableView::DoubleClicked |
									 QTableView::EditKeyPressed
								    : QTableView::NoEditTriggers);
}

void FileWidget::updateView(const QVariantMap& Map)
{
	if (Map.contains("path"))
	{
		const bool En = Map.value("path").toBool();

		if (En) hiddenCols.remove(1);
		else hiddenCols.insert(1);
	}

	if (Map.contains("role"))
	{
		const bool En = Map.value("role").toBool();

		if (En) hiddenCols.remove(3);
		else hiddenCols.insert(3);
	}

	if (Map.contains("comm"))
	{
		const bool En = Map.value("comm").toBool();

		if (En) hiddenCols.remove(4);
		else hiddenCols.insert(4);
	}

	for (int i = 0; i < ui->tableView->model()->columnCount(); ++i)
		ui->tableView->setColumnHidden(i, hiddenCols.contains(i));
}

void FileWidget::editData(const QVariantMap& Map)
{
	const auto S = ui->tableView->selectionModel()->selectedIndexes().first();

	model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);

	filter->setData(filter->index(S.row(), 2, S.parent()), Map[tr("Name")]);
	filter->setData(filter->index(S.row(), 3, S.parent()), Map[tr("Role")]);
	filter->setData(filter->index(S.row(), 4, S.parent()), Map[tr("Comment")]);

	model->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
	model->submitAll();
}

void FileWidget::appendData(const QVariantMap& Map)
{
	emit onAddRow(
	{
		0,
		Map[tr("Path")].toString().simplified().replace("\\", "/"),
		Map[tr("Name")].toString().simplified(),
		Map[tr("Role")].toInt(),
		Map[tr("Comment")].toString().simplified()
	});
}

void FileWidget::rowSelected(const QModelIndex& Index)
{
	if (!model) return;

	const auto i = filter->index(Index.row(), 0, Index.parent());
	const auto p = filter->index(Index.row(), 1, Index.parent());
	const auto f = filter->index(Index.row(), 2, Index.parent());

	emit onIndexChange(i.data().toInt());
	emit onFilepathChange(p.data().toString() + "/" + f.data().toString());
}

void FileWidget::itemSelected(const QItemSelection& Index)
{
	ui->editButton->setEnabled(!Index.isEmpty());
	ui->remButton->setEnabled(!Index.isEmpty());
}

void FileWidget::rowUpdated(const QModelIndex& Index, const QVariant& Old, const QVariant& New)
{
	if (Index.column() != 2 || Old == New) return;

	const auto i = filter->index(Index.row(), 1, Index.parent());
	const QString p = filter->data(i).toString();

	emit onFileRename(p + '/' + Old.toString(), New.toString());
}

void FileWidget::editClicked(void)
{
	const auto S = ui->tableView->selectionModel()->selectedIndexes().first();

	EditDialog* Dialog = new EditDialog(this);
	QSqlQuery Query(Database);
	QVariantMap Map;

	Query.prepare("SELECT nazwa, id "
			    "FROM rodzajedok "
			    "ORDER BY nazwa ASC");

	if (Query.exec()) while (Query.next())
	{
		Map.insert(Query.value(0).toString(), Query.value(1));
	}

	Dialog->appendEdit(tr("Name"), filter->index(S.row(), 2, S.parent()).data().toString());
	Dialog->appendCombo(tr("Role"), Map, filter->index(S.row(), 3, S.parent()).data());
	Dialog->appendEdit(tr("Comment"), filter->index(S.row(), 4, S.parent()).data().toString());

	Dialog->setValidator([] (const QVariantMap& Map) -> bool
	{
		return !Map.value(tr("Name")).toString().simplified().isEmpty();
	});

	Dialog->open();

	connect(Dialog, &EditDialog::onAccept, this, &FileWidget::editData);
	connect(Dialog, &EditDialog::accepted, Dialog, &EditDialog::deleteLater);
	connect(Dialog, &EditDialog::rejected, Dialog, &EditDialog::deleteLater);
}

void FileWidget::addClicked(void)
{
	EditDialog* Dialog = new EditDialog(this);
	QSqlQuery Query(Database);
	QVariantMap Map;

	Query.prepare("SELECT nazwa, id "
			    "FROM rodzajedok "
			    "ORDER BY nazwa ASC");

	if (Query.exec()) while (Query.next())
	{
		Map.insert(Query.value(0).toString(), Query.value(1));
	}

	Dialog->appendEdit(tr("Path"));
	Dialog->appendEdit(tr("Name"));
	Dialog->appendCombo(tr("Role"), Map);
	Dialog->appendEdit(tr("Comment"));

	Dialog->setValidator([] (const QVariantMap& Map) -> bool
	{
		return
			!Map.value(tr("Path")).toString().simplified().isEmpty() &&
			!Map.value(tr("Name")).toString().simplified().isEmpty();
	});

	Dialog->open();

	connect(Dialog, &EditDialog::onAccept, this, &FileWidget::appendData);
	connect(Dialog, &EditDialog::accepted, Dialog, &EditDialog::deleteLater);
	connect(Dialog, &EditDialog::rejected, Dialog, &EditDialog::deleteLater);
}

void FileWidget::remClicked(void)
{
	const auto S = ui->tableView->selectionModel()->selectedIndexes().first();
	emit onRemRow(filter->index(S.row(), 0, S.parent()).data().toInt());
}
