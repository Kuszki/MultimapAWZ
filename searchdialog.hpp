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

#ifndef SEARCHDIALOG_HPP
#define SEARCHDIALOG_HPP

#include <QPushButton>
#include <QVariant>
#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui { class SearchDialog; }
QT_END_NAMESPACE

class SearchDialog : public QDialog
{

		Q_OBJECT

	private:

		QSqlDatabase& Database;

		Ui::SearchDialog *ui;

	public:

		explicit SearchDialog(QSqlDatabase& Db, QWidget *parent = nullptr);
		virtual ~SearchDialog(void) override;

	public slots:

		virtual void open(void) override;
		virtual void accept(void) override;

	private slots:

		void commIndexChanged(int Index);
		void precCheckToggled(bool Enable);
		void commCheckToggled(bool Enable);

		void validateData(void);

	signals:

		void onAccept(const QVariantMap&);
		void onReset(void);

};

#endif // SEARCHDIALOG_HPP
