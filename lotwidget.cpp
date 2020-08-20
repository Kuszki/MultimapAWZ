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
	ui->setupUi(this);

	filter = new ModelFilter(this);
	filter->setSearchedColumns({ 1, 2, 3 });
	filter->setReadonlyColumns({ 0 });

	ui->tableView->model()->deleteLater();
	ui->tableView->setModel(filter);

	connect(ui->searchEdit, &QLineEdit::textChanged,
		   filter, &ModelFilter::setFilterFixedString);

	connect(ui->tableView->selectionModel(),
		   &QItemSelectionModel::currentRowChanged,
		   this, &LotWidget::rowSelected);
}

LotWidget::~LotWidget()
{
	delete ui;
}

void LotWidget::setTitleWidget(TitleWidget* W)
{
	ui->horizontalLayout->removeWidget(ui->reloadButton);
	W->addRightWidget(ui->reloadButton);

	ui->horizontalLayout->removeWidget(ui->editButton);
	W->addRightWidget(ui->editButton);
}

void LotWidget::prepareList(int ID)
{
	QHash<QString, QVariantList> List;
	QSqlQuery Query(Database);

	ui->placeCombo->clear();

	Query.prepare("SELECT d.id, g.nazwa || '/' || o.nazwa "
			    "FROM dok_dzialki j "
			    "INNER JOIN dzialki d ON j.id_dzi = d.id "
			    "LEFT JOIN obreby o ON d.id_obrebu = o.id "
			    "LEFT JOIN gminy g ON o.id_gminy = g.id "
			    "WHERE j.id_dok = ?");
	Query.addBindValue(ID);

	if (Query.exec()) while (Query.next())
	{
		List[Query.value(1).toString()].append(Query.value(0));
	}

	for (auto i = List.constBegin(); i != List.constEnd(); ++i)
	{
		ui->placeCombo->addItem(i.key(), i.value());
	}

	ui->placeCombo->model()->sort(0);
	ui->placeCombo->insertItem(0, tr("All areas"));
	ui->placeCombo->setCurrentIndex(0);
}

void LotWidget::filterList(int ID)
{
	prepareList(lastIndex = ID);

	if (model) model->setFilter(filterStr.arg(ID));
	if (model && !model->rowCount()) model->select();
}

void LotWidget::reloadList(void)
{
	if (model)
	{
		prepareList(lastIndex);
		model->select();
	}
}

void LotWidget::selectComm(int Index)
{
	const QVariantList List = ui->placeCombo->itemData(Index).toList();

	QSet<int> S; for (const auto& i : List) S.insert(i.toInt());

	filter->setFilterIndexes(S);
}

void LotWidget::setStatus(bool Enabled)
{
	ui->tableView->setEnabled(Enabled);
	ui->editButton->setEnabled(Enabled);
	ui->reloadButton->setEnabled(Enabled);
	ui->searchEdit->setEnabled(Enabled);
	ui->placeCombo->setEnabled(Enabled);

	if (Enabled)
	{
		model = new QSqlTableModel(this, Database);

		model->setTable("DZIALKI");

		model->setHeaderData(0, Qt::Horizontal, tr("ID"));
		model->setHeaderData(1, Qt::Horizontal, tr("Number"));
		model->setHeaderData(2, Qt::Horizontal, tr("Sheet"));
		model->setHeaderData(3, Qt::Horizontal, tr("Precinct ID"));

		model->setEditStrategy(QSqlTableModel::OnFieldChange);

		filter->setSourceModel(model);

		ui->tableView->hideColumn(0);
		ui->tableView->hideColumn(3);
	}
	else if (model)
	{
		ui->placeCombo->clear();
		filter->setSourceModel(nullptr);

		model->deleteLater();
		model = nullptr;
	}
}

void LotWidget::rowSelected(const QModelIndex& Index)
{

}

void LotWidget::editClicked(void)
{
	// TODO Implement me
}
