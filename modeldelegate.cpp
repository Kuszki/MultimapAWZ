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

#include "modeldelegate.hpp"

ModelDelegate::ModelDelegate(QObject *parent)
: QSqlRelationalDelegate(parent) {}

ModelDelegate::~ModelDelegate(void) {}

QWidget* ModelDelegate::createEditor(QWidget* aParent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const QSqlRelationalTableModel *sqlModel = qobject_cast<const QSqlRelationalTableModel*>(index.model());
	QSqlTableModel *childModel = sqlModel ? sqlModel->relationModel(index.column()) : nullptr;

	if (!childModel)
	{
		const QAbstractProxyModel* proxyModel = qobject_cast<const QAbstractProxyModel*>(index.model());

		if (proxyModel)
		{
			sqlModel = qobject_cast<const QSqlRelationalTableModel*>(proxyModel->sourceModel());
			childModel = sqlModel ? sqlModel->relationModel(index.column()) : nullptr;
		}
	}

	if (!childModel)
	{
		return QItemDelegate::createEditor(aParent, option, index);
	}

	QComboBox *combo = new QComboBox(aParent);

	combo->setModel(childModel);
	combo->setModelColumn(childModel->fieldIndex(sqlModel->relation(index.column()).displayColumn()));
	combo->installEventFilter(const_cast<ModelDelegate*>(this));

	return combo;
}

void ModelDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	if (!index.isValid()) return;

	QSqlRelationalTableModel *sqlModel = qobject_cast<QSqlRelationalTableModel*>(model);
	QAbstractProxyModel* proxyModel = nullptr;

	if (!sqlModel)
	{
		proxyModel = qobject_cast<QAbstractProxyModel*>(model);

		if (proxyModel)
			sqlModel = qobject_cast<QSqlRelationalTableModel*>(proxyModel->sourceModel());
	}

	QSqlTableModel *childModel = sqlModel ? sqlModel->relationModel(index.column()) : nullptr;
	QComboBox *combo = qobject_cast<QComboBox *>(editor);

	if (!sqlModel || !childModel || !combo)
	{
		QItemDelegate::setModelData(editor, model, index); return;
	}

	int currentItem = combo->currentIndex();
	int childColIndex = childModel->fieldIndex(sqlModel->relation(index.column()).displayColumn());
	int childEditIndex = childModel->fieldIndex(sqlModel->relation(index.column()).indexColumn());

	if (proxyModel)
	{
		proxyModel->setData(index, childModel->data(childModel->index(currentItem, childColIndex), Qt::DisplayRole), Qt::DisplayRole);
		proxyModel->setData(index, childModel->data(childModel->index(currentItem, childEditIndex), Qt::EditRole), Qt::EditRole);
	}
	else
	{
		sqlModel->setData(index, childModel->data(childModel->index(currentItem, childColIndex), Qt::DisplayRole), Qt::DisplayRole);
		sqlModel->setData(index, childModel->data(childModel->index(currentItem, childEditIndex), Qt::EditRole), Qt::EditRole);
	}
}

void ModelDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	QString strVal;

	const QSqlRelationalTableModel *sqlModel = qobject_cast<const QSqlRelationalTableModel*>(index.model());

	if (!sqlModel)
	{
		const QAbstractProxyModel* proxyModel = qobject_cast<const QAbstractProxyModel*>(index.model());

		if (proxyModel)
		{
			strVal = proxyModel->data(index).toString();
		}
	}
	else
	{
		strVal = sqlModel->data(index).toString();
	}

	QComboBox *combo = qobject_cast<QComboBox*>(editor);

	if (strVal.isEmpty() || !combo)
	{
		QItemDelegate::setEditorData(editor, index); return;
	}

	combo->setCurrentIndex(combo->findText(strVal));
}
