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

#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include <QDialog>
#include <QSettings>
#include <QVariant>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {	class SettingsDialog; }
QT_END_NAMESPACE

class SettingsDialog : public QDialog
{

		Q_OBJECT

	private:

		Ui::SettingsDialog *ui;

	public:

		explicit SettingsDialog(QWidget* parent = nullptr);
		virtual ~SettingsDialog(void) override;

	public slots:

		virtual void accept(void) override;

		void reset(void);
		void save(void);

	signals:

		void onAccept(const QVariantMap&);

};

#endif // SETTINGSDIALOG_HPP
