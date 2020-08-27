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

#ifndef AWZWIDGET_HPP
#define AWZWIDGET_HPP

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

#include "modelfilter.hpp"
#include "titlewidget.hpp"
#include "editdialog.hpp"

#include "commonh.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class AwzWidget; }
QT_END_NAMESPACE

class AwzWidget : public QWidget
{

		Q_OBJECT

	private:

		QSqlDatabase& Database;
		QSet<int> hiddenCols;

		ModelFilter* filter = nullptr;
		QSqlTableModel* model = nullptr;

		Ui::AwzWidget *ui;

	public:

		explicit AwzWidget(QSqlDatabase& Db,
					    QWidget *parent = nullptr);
		virtual ~AwzWidget(void) override;

		void setTitleWidget(TitleWidget* W);

	public slots:

		void filterList(const QSet<int>& List);

		void reloadList(void);

		void setStatus(bool Enabled);
		void setEditable(bool Enabled);

		void updateView(const QVariantMap& Map);

	private slots:

		void editData(const QVariantMap& Map);
		void appendData(const QVariantMap& Map);

		void rowSelected(const QModelIndex& Index);
		void itemSelected(const QItemSelection& Index);

		void editClicked(void);
		void addClicked(void);
		void remClicked(void);

	signals:

		void onIndexChange(int);
		void onDirChange(const QString&);

		void onAddRow(const DOKUMENTY&);
		void onRemRow(int);

};

#endif // AWZWIDGET_HPP
