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

#ifndef PROGRESSDIALOG_HPP
#define PROGRESSDIALOG_HPP

#include <QPushButton>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {	class ProgressDialog; }
QT_END_NAMESPACE

class ProgressDialog : public QDialog
{
		Q_OBJECT

	private:

		Ui::ProgressDialog *ui;

		bool isAction = false;

	public:

		explicit ProgressDialog(QWidget *parent = nullptr);
		virtual ~ProgressDialog(void) override;

	public slots:

		virtual void accept(void) override;
		virtual void reject(void) override;
		virtual void open(void) override;

		void startAction(const QString& Action);

		void startProcess(int a, int b);
		void updateProcess(int n);

		void startSubprocess(int a, int b);
		void updateSubprocess(int n);

		void endJob(const QString& Msg);
};

#endif // PROGRESSDIALOG_HPP
