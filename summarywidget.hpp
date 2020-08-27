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

#ifndef SUMMARYWIDGET_HPP
#define SUMMARYWIDGET_HPP

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileInfo>
#include <QDesktopServices>
#include <QToolButton>
#include <QUrl>
#include <QDir>

#include "titlewidget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {	class SummaryWidget; }
QT_END_NAMESPACE

class SummaryWidget : public QWidget
{

		Q_OBJECT

	private:

		QString Path, File, Dir;
		QSqlDatabase& Database;

		QToolButton* file;
		QToolButton* dir;

		Ui::SummaryWidget *ui;

		int lastID = 0;

	public:

		explicit SummaryWidget(QSqlDatabase& Db, QWidget *parent = nullptr);
		virtual ~SummaryWidget(void) override;

		void setTitleWidget(TitleWidget* W);

	public slots:

		void updateFile(const QString& Src);
		void updateDir(const QString& Src);

		void setPath(const QString& P);

		void filterList(int ID);

		void reloadList(void);

		void setStatus(bool Enabled);

	private slots:

		void dirButtonClicked(void);
		void fileButtonClicked(void);

};

#endif // SUMMARYWIDGET_HPP
