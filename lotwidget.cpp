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

#include "lotwidget.hpp"
#include "ui_lotwidget.h"

const QString LotWidget::filterStr = "DZIALKI.ID IN (SELECT J.ID_DZI FROM DOK_DZIALKI J WHERE J.ID_DOK = %1)";

LotWidget::LotWidget(QSqlDatabase& Db, QWidget *parent)
: QWidget(parent), Database(Db), ui(new Ui::LotWidget)
{
	ui->setupUi(this); setEditable(false);

	hiddenCols = { 0, 2 };

	QSettings Settings("Multimap", "AWZ");

	Settings.beginGroup("Lots");
	if (Settings.value("Sheet", true).toBool()) hiddenCols.remove(2);
	Settings.endGroup();

	filter = new ModelFilter(this);
	filter->setSearchedColumns({ 1, 2, 3 });
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
		   this, &LotWidget::rowSelected);

	connect(ui->tableView->selectionModel(),
		   &QItemSelectionModel::selectionChanged,
		   this, &LotWidget::itemSelected);
}

LotWidget::~LotWidget(void)
{
	delete ui;
}

void LotWidget::setTitleWidget(TitleWidget* W)
{
	ui->horizontalLayout->removeWidget(ui->editButton);
	W->addRightWidget(ui->editButton);

	ui->horizontalLayout->removeWidget(ui->addButton);
	W->addRightWidget(ui->addButton);

	ui->horizontalLayout->removeWidget(ui->remButton);
	W->addRightWidget(ui->remButton);
}

void LotWidget::filterList(int ID)
{
	ui->addButton->setEnabled(ID != -1);

	ui->editButton->setEnabled(false);
	ui->remButton->setEnabled(false);

	if (model) model->setFilter(filterStr.arg(ID));
	if (model && !model->rowCount()) model->select();
}

void LotWidget::reloadList(void)
{
	ui->editButton->setEnabled(false);
	ui->remButton->setEnabled(false);

	if (model) model->select();

	emit onIndexChange(-1);
}

void LotWidget::setStatus(bool Enabled)
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

		model->setTable("DZIALKI");

		model->setRelation(3, QSqlRelation("OBRGMINY", "ID", "NAZWA"));

		model->setHeaderData(0, Qt::Horizontal, tr("ID"));
		model->setHeaderData(1, Qt::Horizontal, tr("Number"));
		model->setHeaderData(2, Qt::Horizontal, tr("Sheet"));
		model->setHeaderData(3, Qt::Horizontal, tr("Precinct"));

		model->setEditStrategy(QSqlTableModel::OnFieldChange);

		filter->setSourceModel(model);

		ui->tableView->setItemDelegate(new ModelDelegate(this));

		for (const auto& i : hiddenCols) ui->tableView->hideColumn(i);
	}
	else if (model)
	{
		filter->setSourceModel(nullptr);

		model->deleteLater();
		model = nullptr;
	}
}

void LotWidget::setEditable(bool Enabled)
{
	ui->addButton->setVisible(Enabled);
	ui->remButton->setVisible(Enabled);
	ui->editButton->setVisible(Enabled);

	ui->tableView->setEditTriggers(Enabled ? QTableView::DoubleClicked |
									 QTableView::EditKeyPressed
								    : QTableView::NoEditTriggers);
}

void LotWidget::updateView(const QVariantMap& Map)
{
	if (Map.contains("sheet"))
	{
		const bool En = Map.value("sheet").toBool();

		if (En) hiddenCols.remove(2);
		else hiddenCols.insert(2);
	}

	for (int i = 0; i < ui->tableView->model()->columnCount(); ++i)
		ui->tableView->setColumnHidden(i, hiddenCols.contains(i));
}

void LotWidget::editData(const QVariantMap& Map)
{
	const auto S = ui->tableView->selectionModel()->selectedIndexes().first();

	model->setEditStrategy(QSqlTableModel::OnManualSubmit);

	filter->setData(filter->index(S.row(), 1, S.parent()), Map[tr("Number")]);
	filter->setData(filter->index(S.row(), 2, S.parent()), Map[tr("Sheet")]);
	filter->setData(filter->index(S.row(), 3, S.parent()), Map[tr("Precinct")]);

	model->submitAll();

	model->setEditStrategy(QSqlTableModel::OnFieldChange);
}

void LotWidget::appendData(const QVariantMap& Map)
{
	emit onAddRow(
	{
		0,
		Map[tr("Number")].toString().simplified(),
		Map[tr("Sheet")].toString().simplified(),
		Map[tr("Precinct")].toInt()
	});
}

void LotWidget::rowSelected(const QModelIndex& Index)
{
	if (!model) return;

	const auto i = filter->index(Index.row(), 0, Index.parent());

	emit onIndexChange(i.data().toInt());
}

void LotWidget::itemSelected(const QItemSelection& Index)
{
	ui->editButton->setEnabled(!Index.isEmpty());
	ui->remButton->setEnabled(!Index.isEmpty());
}

void LotWidget::editClicked(void)
{
	const auto S = ui->tableView->selectionModel()->selectedIndexes().first();

	EditDialog* Dialog = new EditDialog(this);
	QSqlQuery Query(Database);
	QVariantMap Map;

	Query.prepare("SELECT id, nazwa FROM obrgminy");

	if (Query.exec()) while (Query.next())
	{
		Map.insert(Query.value(1).toString(), Query.value(0));
	}

	Dialog->appendEdit(tr("Number"), filter->index(S.row(), 1, S.parent()).data().toString());
	Dialog->appendEdit(tr("Sheet"), filter->index(S.row(), 2, S.parent()).data().toString());
	Dialog->appendCombo(tr("Precinct"), Map, filter->index(S.row(), 3, S.parent()).data().toString());

	Dialog->setValidator([] (const QVariantMap& Map) -> bool
	{
		return !Map.value(tr("Number")).toString().simplified().isEmpty();
	});

	Dialog->open();

	connect(Dialog, &EditDialog::onAccept, this, &LotWidget::editData);
	connect(Dialog, &EditDialog::accepted, Dialog, &EditDialog::deleteLater);
	connect(Dialog, &EditDialog::rejected, Dialog, &EditDialog::deleteLater);
}

void LotWidget::addClicked(void)
{
	EditDialog* Dialog = new EditDialog(this);
	QSqlQuery Query(Database);
	QVariantMap Map;

	Query.prepare("SELECT id, nazwa FROM obrgminy");

	if (Query.exec()) while (Query.next())
	{
		Map.insert(Query.value(1).toString(), Query.value(0));
	}

	Dialog->appendEdit(tr("Number"));
	Dialog->appendEdit(tr("Sheet"));
	Dialog->appendCombo(tr("Precinct"), Map);

	Dialog->setValidator([] (const QVariantMap& Map) -> bool
	{
		return !Map.value(tr("Number")).toString().simplified().isEmpty();
	});

	Dialog->open();

	connect(Dialog, &EditDialog::onAccept, this, &LotWidget::appendData);
	connect(Dialog, &EditDialog::accepted, Dialog, &EditDialog::deleteLater);
	connect(Dialog, &EditDialog::rejected, Dialog, &EditDialog::deleteLater);
}

void LotWidget::remClicked(void)
{
	const auto S = ui->tableView->selectionModel()->selectedIndexes().first();
	emit onRemRow(filter->index(S.row(), 0, S.parent()).data().toInt());
}
