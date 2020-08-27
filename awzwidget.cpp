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
	ui->setupUi(this); setEditable(false);

	hiddenCols = { 0, 3 };

	QSettings Settings("Multimap", "AWZ");

	Settings.beginGroup("Documents");
	if (Settings.value("Comment", false).toBool()) hiddenCols.remove(2);
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
		   this, &AwzWidget::rowSelected);

	connect(ui->tableView->selectionModel(),
		   &QItemSelectionModel::selectionChanged,
		   this, &AwzWidget::itemSelected);
}

AwzWidget::~AwzWidget(void)
{
	delete ui;
}

void AwzWidget::setTitleWidget(TitleWidget* W)
{
	ui->horizontalLayout->removeWidget(ui->editButton);
	W->addRightWidget(ui->editButton);

	ui->horizontalLayout->removeWidget(ui->addButton);
	W->addRightWidget(ui->addButton);

	ui->horizontalLayout->removeWidget(ui->remButton);
	W->addRightWidget(ui->remButton);
}

void AwzWidget::filterList(const QSet<int>& List)
{
	ui->editButton->setEnabled(false);
	ui->remButton->setEnabled(false);

	if (model) filter->setFilterIndexes(List);
	if (model && !model->rowCount()) model->select();
}

void AwzWidget::reloadList(void)
{
	ui->editButton->setEnabled(false);
	ui->remButton->setEnabled(false);

	if (model) model->select();

	emit onIndexChange(-1);
}

void AwzWidget::setStatus(bool Enabled)
{
	ui->tableView->setEnabled(Enabled);
	ui->reloadButton->setEnabled(Enabled);
	ui->addButton->setEnabled(Enabled);
	ui->searchEdit->setEnabled(Enabled);

	ui->editButton->setEnabled(false);
	ui->remButton->setEnabled(false);

	if (Enabled)
	{
		model = new QSqlTableModel(this, Database);

		model->setTable("DOKUMENTY");

		model->setHeaderData(0, Qt::Horizontal, tr("ID"));
		model->setHeaderData(1, Qt::Horizontal, tr("Name"));
		model->setHeaderData(2, Qt::Horizontal, tr("Mark"));
		model->setHeaderData(3, Qt::Horizontal, tr("Comments"));

		model->setEditStrategy(QSqlTableModel::OnFieldChange);

		model->select();

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

void AwzWidget::setEditable(bool Enabled)
{
	ui->addButton->setVisible(Enabled);
	ui->remButton->setVisible(Enabled);
	ui->editButton->setVisible(Enabled);

	ui->tableView->setEditTriggers(Enabled ? QTableView::DoubleClicked |
									 QTableView::EditKeyPressed
								    : QTableView::NoEditTriggers);
}

void AwzWidget::updateView(const QVariantMap& Map)
{
	if (Map.contains("comm"))
	{
		const bool En = Map.value("comm").toBool();

		if (En) hiddenCols.remove(3);
		else hiddenCols.insert(3);
	}

	for (int i = 0; i < ui->tableView->model()->columnCount(); ++i)
		ui->tableView->setColumnHidden(i, hiddenCols.contains(i));
}

void AwzWidget::editData(const QVariantMap& Map)
{
	const auto S = ui->tableView->selectionModel()->selectedIndexes().first();

	model->setEditStrategy(QSqlTableModel::OnManualSubmit);

	filter->setData(filter->index(S.row(), 1, S.parent()), Map[tr("Name")]);
	filter->setData(filter->index(S.row(), 2, S.parent()), Map[tr("Mark")]);
	filter->setData(filter->index(S.row(), 3, S.parent()), Map[tr("Comment")]);

	model->setEditStrategy(QSqlTableModel::OnFieldChange);
	model->submitAll();
}

void AwzWidget::appendData(const QVariantMap& Map)
{
	emit onAddRow(
	{
		0,
		Map[tr("Name")].toString().simplified(),
		Map[tr("Mark")].toString().simplified(),
		Map[tr("Comment")].toString().simplified()
	});
}

void AwzWidget::rowSelected(const QModelIndex& Index)
{
	if (!model) return;

	const auto id = filter->index(Index.row(), 0, Index.parent()).data();

	emit onIndexChange(id.toInt());
}

void AwzWidget::itemSelected(const QItemSelection& Index)
{
	ui->editButton->setEnabled(!Index.isEmpty());
	ui->remButton->setEnabled(!Index.isEmpty());
}

void AwzWidget::editClicked(void)
{
	const auto S = ui->tableView->selectionModel()->selectedIndexes().first();

	EditDialog* Dialog = new EditDialog(this);

	Dialog->appendEdit(tr("Name"), filter->index(S.row(), 1, S.parent()).data().toString());
	Dialog->appendEdit(tr("Mark"), filter->index(S.row(), 2, S.parent()).data().toString());
	Dialog->appendEdit(tr("Comment"), filter->index(S.row(), 3, S.parent()).data().toString());

	Dialog->setValidator([] (const QVariantMap& Map) -> bool
	{
		return !Map.value(tr("Name")).toString().simplified().isEmpty();
	});

	Dialog->open();

	connect(Dialog, &EditDialog::onAccept, this, &AwzWidget::editData);
	connect(Dialog, &EditDialog::accepted, Dialog, &EditDialog::deleteLater);
	connect(Dialog, &EditDialog::rejected, Dialog, &EditDialog::deleteLater);
}

void AwzWidget::addClicked(void)
{
	EditDialog* Dialog = new EditDialog(this);

	Dialog->appendEdit(tr("Name"));
	Dialog->appendEdit(tr("Mark"));
	Dialog->appendEdit(tr("Comment"));

	Dialog->setValidator([] (const QVariantMap& Map) -> bool
	{
		return !Map.value(tr("Name")).toString().simplified().isEmpty();
	});

	Dialog->open();

	connect(Dialog, &EditDialog::onAccept, this, &AwzWidget::appendData);
	connect(Dialog, &EditDialog::accepted, Dialog, &EditDialog::deleteLater);
	connect(Dialog, &EditDialog::rejected, Dialog, &EditDialog::deleteLater);
}

void AwzWidget::remClicked(void)
{
	const auto S = ui->tableView->selectionModel()->selectedIndexes().first();
	emit onRemRow(filter->index(S.row(), 0, S.parent()).data().toInt());
}
