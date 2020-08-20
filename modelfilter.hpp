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

#ifndef MODELFILTER_HPP
#define MODELFILTER_HPP

#include <QSortFilterProxyModel>
#include <QModelIndex>
#include <QObject>
#include <QSet>

#include <QDebug>

class ModelFilter : public QSortFilterProxyModel
{

		Q_OBJECT

	private:

		QSet<int> Columns;
		QSet<int> Indexes;

		QSet<int> Readonly;

	public:

		explicit ModelFilter(QObject* parent = nullptr);
		virtual ~ModelFilter(void) override;

		void setSearchedColumns(const QSet<int>& Set);
		void setFilterIndexes(const QSet<int>& Set);

		QSet<int> getReadonlyColumns(void) const;
		void setReadonlyColumns(const QSet<int>& R);

	public slots:

		virtual bool setData(const QModelIndex &Index,
						 const QVariant &Value, int Role) override;

	protected:

		virtual bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

	signals:

		void onRecordUpdate(const QModelIndex&,
						const QVariant&,
						const QVariant&);

};

#endif // MODELFILTER_HPP
