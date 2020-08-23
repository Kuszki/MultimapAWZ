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

#ifndef FILEWIDGET_HPP
#define FILEWIDGET_HPP

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlRelationalDelegate>
#include <QSqlRelationalTableModel>
#include <QFile>

#include "modelfilter.hpp"
#include "modeldelegate.hpp"
#include "titlewidget.hpp"
#include "editdialog.hpp"

#include "commonh.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class FileWidget; }
QT_END_NAMESPACE

class FileWidget : public QWidget
{

		Q_OBJECT

	private:

		static const QString filterStr;

		QSqlDatabase& Database;

		ModelFilter* filter = nullptr;
		QSqlRelationalTableModel* model = nullptr;

		Ui::FileWidget *ui;

	public:

		explicit FileWidget(QSqlDatabase& Db,
						QWidget *parent = nullptr);
		virtual ~FileWidget(void) override;

		void setTitleWidget(TitleWidget* W);

	public slots:

		void filterList(int ID);

		void reloadList(void);

		void setStatus(bool Enabled);

	private slots:

		void editData(const QVariantMap& Map);
		void appendData(const QVariantMap& Map);

		void rowSelected(const QModelIndex& Index);
		void itemSelected(const QItemSelection& Index);

		void rowUpdated(const QModelIndex& Index,
					 const QVariant& Old,
					 const QVariant& New);

		void editClicked(void);
		void addClicked(void);
		void remClicked(void);

	signals:

		void onIndexChange(int);

		void onFilepathChange(const QString&);

		void onFileRename(const QString&,
					   const QString&);

		void onAddRow(const PLIKI&);
		void onRemRow(int);

};

#endif // FILEWIDGET_HPP
