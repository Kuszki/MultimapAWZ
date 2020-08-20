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

#ifndef OWNERWIDGET_HPP
#define OWNERWIDGET_HPP

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

#include "modelfilter.hpp"
#include "modeldelegate.hpp"
#include "titlewidget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class OwnerWidget; }
QT_END_NAMESPACE

class OwnerWidget : public QWidget
{

		Q_OBJECT

	private:

		static const QString filterStr;

		QSqlDatabase& Database;

		ModelFilter* filter = nullptr;
		QSqlTableModel* model = nullptr;

		Ui::OwnerWidget *ui;

	public:

		explicit OwnerWidget(QSqlDatabase& Db,
						 QWidget *parent = nullptr);
		virtual ~OwnerWidget(void) override;

		void setTitleWidget(TitleWidget* W);

	public slots:

		void filterList(int ID);

		void reloadList(void);

		void setStatus(bool Enabled);

	private slots:

		void rowSelected(const QModelIndex& Index);

		void editClicked(void);

	signals:

		void onIndexChange(int);

};

#endif // OWNERWIDGET_HPP
