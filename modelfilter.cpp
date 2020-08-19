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

#include "modelfilter.hpp"

ModelFilter::ModelFilter(QObject* parent)
: QSortFilterProxyModel(parent)
{
	this->setFilterCaseSensitivity(Qt::CaseInsensitive);
}

ModelFilter::~ModelFilter(void) {}

void ModelFilter::setSearchedColumns(const QSet<int>& Set)
{
	Columns = Set;

	invalidateFilter();
}

void ModelFilter::setFilterIndexes(const QSet<int>& Set)
{
	Indexes = Set;

	invalidateFilter();
}

bool ModelFilter::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
	QModelIndex idIndex = sourceModel()->index(sourceRow, 0, sourceParent);

	const bool isID = Indexes.isEmpty() || Indexes.contains(sourceModel()->data(idIndex).toInt());
	bool isAny = Columns.isEmpty();

	for (const auto& i : Columns)
	{
		QModelIndex fIndex = sourceModel()->index(sourceRow, i, sourceParent);

		isAny = isAny || sourceModel()->data(fIndex).toString().contains(filterRegExp());
	}

	return isID && isAny;
}
