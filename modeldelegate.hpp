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

#ifndef MODELDELEGATE_HPP
#define MODELDELEGATE_HPP

#include <QObject>
#include <QSortFilterProxyModel>
#include <QSqlRelationalDelegate>

class ModelDelegate : public QSqlRelationalDelegate
{

		Q_OBJECT

	public:

		explicit ModelDelegate(QObject *parent = nullptr);
		virtual ~ModelDelegate(void) override;

		QWidget* createEditor(QWidget *aParent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
		void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
		void setEditorData(QWidget *editor, const QModelIndex &index) const override;

};

#endif // MODELDELEGATE_HPP
