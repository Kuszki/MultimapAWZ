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

#ifndef IMPORTDIALOG_HPP
#define IMPORTDIALOG_HPP

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QSet>
#include <QFileDialog>

#include "commonh.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class ImportDialog; }
QT_END_NAMESPACE

class ImportDialog : public QDialog
{

		Q_OBJECT

	private:

		Ui::ImportDialog *ui;

	public:

		explicit ImportDialog(QWidget* parent = nullptr);
		virtual ~ImportDialog(void) override;

	public slots:

		virtual void accept(void) override;

	private slots:

		void checkData(void);

		void openDocument(void);
		void openFile(void);
		void openDict(void);

	signals:

		void onDocAccept(const QString&, const QMap<ROLES, int>&,
					  const QString&, const QString&);

		void onFileAccept(const QString&, const QMap<ROLES, int>&,
					   const QString&);

		void onDictAccept(const QString&, const QMap<ROLES, int>&,
					   const QString&);

};

#endif // IMPORTDIALOG_HPP
