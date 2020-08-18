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

	connect(ui->searchEdit, &QLineEdit::textChanged,
		   this, &AwzWidget::searchEditChanged);

	QSqlTableModel* model = new QSqlTableModel(this, Db);

	model->setTable("dokumenty");
	model->setHeaderData(0, Qt::Horizontal, tr("ID"));
	model->setHeaderData(1, Qt::Horizontal, tr("Name"));
	model->select();

	ui->listView->setModel(model);
	ui->listView->setModelColumn(1);
}

AwzWidget::~AwzWidget(void)
{
	delete ui;
}

void AwzWidget::reloadList(void)
{
	QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(ui->listView->model());

	model->select();
}

void AwzWidget::filterList(const QSet<int>& List)
{

}

void AwzWidget::searchEditChanged(const QString& Text)
{
	QSqlTableModel* model = dynamic_cast<QSqlTableModel*>(ui->listView->model());
}
