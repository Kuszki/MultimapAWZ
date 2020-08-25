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
	ui->setupUi(this); setEditable(false);

	hiddenCols = { 0, 3, 4 };

	QSettings Settings("Multimap", "AWZ");

	Settings.beginGroup("Owners");
	if (Settings.value("Father", false).toBool()) hiddenCols.remove(3);
	if (Settings.value("Mother", false).toBool()) hiddenCols.remove(4);
	Settings.endGroup();

	filter = new ModelFilter(this);
	filter->setSearchedColumns({ 1, 2 });
	filter->setReadonlyColumns({ 0 });

	ui->tableView->model()->deleteLater();
	ui->tableView->setModel(filter);

	ui->reloadButton->setFixedSize(
		ui->searchEdit->sizeHint().height(),
		ui->searchEdit->sizeHint().height());

	connect(ui->searchEdit, &QLineEdit::textChanged,
		   filter, &ModelFilter::setFilterFixedString);

	connect(ui->tableView->selectionModel(),
		   &QItemSelectionModel::currentRowChanged,
		   this, &OwnerWidget::rowSelected);

	connect(ui->tableView->selectionModel(),
		   &QItemSelectionModel::selectionChanged,
		   this, &OwnerWidget::itemSelected);
}

OwnerWidget::~OwnerWidget(void)
{
	delete ui;
}

void OwnerWidget::setTitleWidget(TitleWidget* W)
{
	ui->horizontalLayout->removeWidget(ui->editButton);
	W->addRightWidget(ui->editButton);

	ui->horizontalLayout->removeWidget(ui->addButton);
	W->addRightWidget(ui->addButton);

	ui->horizontalLayout->removeWidget(ui->remButton);
	W->addRightWidget(ui->remButton);
}

void OwnerWidget::filterList(int ID)
{
	ui->addButton->setEnabled(ID != -1);

	ui->editButton->setEnabled(false);
	ui->remButton->setEnabled(false);

	if (model) model->setFilter(filterStr.arg(ID));
	if (model && !model->rowCount()) model->select();
}

void OwnerWidget::reloadList(void)
{
	ui->editButton->setEnabled(false);
	ui->remButton->setEnabled(false);

	if (model) model->select();

	emit onIndexChange(-1);
}

void OwnerWidget::setStatus(bool Enabled)
{
	ui->tableView->setEnabled(Enabled);
	ui->reloadButton->setEnabled(Enabled);
	ui->searchEdit->setEnabled(Enabled);

	ui->editButton->setEnabled(false);
	ui->addButton->setEnabled(false);
	ui->remButton->setEnabled(false);

	void editData(const QVariantMap& Map);

	if (Enabled)
	{
		model = new QSqlTableModel(this, Database);

		model->setTable("OSOBY");

		model->setHeaderData(0, Qt::Horizontal, tr("ID"));
		model->setHeaderData(1, Qt::Horizontal, tr("Name"));
		model->setHeaderData(2, Qt::Horizontal, tr("Surname"));
		model->setHeaderData(3, Qt::Horizontal, tr("Father name"));
		model->setHeaderData(4, Qt::Horizontal, tr("Mother name"));

		model->setEditStrategy(QSqlTableModel::OnFieldChange);

		filter->setSourceModel(model);

		for (const auto& i : hiddenCols) ui->tableView->hideColumn(i);
	}
	else if (model)
	{
		filter->setSourceModel(nullptr);

		model->deleteLater();
		model = nullptr;
	}
}

void OwnerWidget::setEditable(bool Enabled)
{
	ui->addButton->setVisible(Enabled);
	ui->remButton->setVisible(Enabled);
	ui->editButton->setVisible(Enabled);

	ui->tableView->setEditTriggers(Enabled ? QTableView::DoubleClicked |
									 QTableView::EditKeyPressed
								    : QTableView::NoEditTriggers);
}

void OwnerWidget::updateView(const QVariantMap& Map)
{
	if (Map.contains("fath"))
	{
		const bool En = Map.value("fath").toBool();

		if (En) hiddenCols.remove(3);
		else hiddenCols.insert(3);
	}

	if (Map.contains("moth"))
	{
		const bool En = Map.value("moth").toBool();

		if (En) hiddenCols.remove(4);
		else hiddenCols.insert(4);
	}

	for (int i = 0; i < ui->tableView->model()->columnCount(); ++i)
		ui->tableView->setColumnHidden(i, hiddenCols.contains(i));
}

void OwnerWidget::editData(const QVariantMap& Map)
{
	const auto S = ui->tableView->selectionModel()->selectedIndexes().first();

	model->setEditStrategy(QSqlTableModel::OnManualSubmit);

	filter->setData(filter->index(S.row(), 1, S.parent()), Map[tr("Name")]);
	filter->setData(filter->index(S.row(), 2, S.parent()), Map[tr("Surname")]);
	filter->setData(filter->index(S.row(), 3, S.parent()), Map[tr("Father name")]);
	filter->setData(filter->index(S.row(), 4, S.parent()), Map[tr("Mother name")]);

	model->setEditStrategy(QSqlTableModel::OnFieldChange);
	model->submitAll();
}

void OwnerWidget::appendData(const QVariantMap& Map)
{
	emit onAddRow(
	{
		0,
		Map[tr("Name")].toString().simplified(),
		Map[tr("Surname")].toString().simplified(),
		Map[tr("Father name")].toString().simplified(),
		Map[tr("Mother name")].toString().simplified()
	});
}

void OwnerWidget::rowSelected(const QModelIndex& Index)
{
	if (!model) return;

	const auto i = filter->index(Index.row(), 0, Index.parent());

	emit onIndexChange(i.data().toInt());
}

void OwnerWidget::itemSelected(const QItemSelection& Index)
{
	ui->editButton->setEnabled(!Index.isEmpty());
	ui->remButton->setEnabled(!Index.isEmpty());
}

void OwnerWidget::editClicked(void)
{
	const auto S = ui->tableView->selectionModel()->selectedIndexes().first();

	EditDialog* Dialog = new EditDialog(this);

	Dialog->appendEdit(tr("Name"), filter->index(S.row(), 1, S.parent()).data().toString());
	Dialog->appendEdit(tr("Surname"), filter->index(S.row(), 2, S.parent()).data().toString());
	Dialog->appendEdit(tr("Father name"), filter->index(S.row(), 3, S.parent()).data().toString());
	Dialog->appendEdit(tr("Mother name"), filter->index(S.row(), 4, S.parent()).data().toString());

	Dialog->setValidator([] (const QVariantMap& Map) -> bool
	{
		return
				!Map.value(tr("Name")).toString().simplified().isEmpty() &&
				!Map.value(tr("Surname")).toString().simplified().isEmpty() &&
				!Map.value(tr("Father name")).toString().simplified().isEmpty() &&
				!Map.value(tr("Mother name")).toString().simplified().isEmpty();
	});

	Dialog->open();

	connect(Dialog, &EditDialog::onAccept, this, &OwnerWidget::editData);
	connect(Dialog, &EditDialog::accepted, Dialog, &EditDialog::deleteLater);
	connect(Dialog, &EditDialog::rejected, Dialog, &EditDialog::deleteLater);
}

void OwnerWidget::addClicked(void)
{
	EditDialog* Dialog = new EditDialog(this);

	Dialog->appendEdit(tr("Name"));
	Dialog->appendEdit(tr("Surname"));
	Dialog->appendEdit(tr("Father name"));
	Dialog->appendEdit(tr("Mother name"));

	Dialog->setValidator([] (const QVariantMap& Map) -> bool
	{
		return
				!Map.value(tr("Name")).toString().simplified().isEmpty() &&
				!Map.value(tr("Surname")).toString().simplified().isEmpty() &&
				!Map.value(tr("Father name")).toString().simplified().isEmpty() &&
				!Map.value(tr("Mother name")).toString().simplified().isEmpty();
	});

	Dialog->open();

	connect(Dialog, &EditDialog::onAccept, this, &OwnerWidget::appendData);
	connect(Dialog, &EditDialog::accepted, Dialog, &EditDialog::deleteLater);
	connect(Dialog, &EditDialog::rejected, Dialog, &EditDialog::deleteLater);
}

void OwnerWidget::remClicked(void)
{
	const auto S = ui->tableView->selectionModel()->selectedIndexes().first();
	emit onRemRow(filter->index(S.row(), 0, S.parent()).data().toInt());
}
