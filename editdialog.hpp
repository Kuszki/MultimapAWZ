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

#ifndef EDITDIALOG_HPP
#define EDITDIALOG_HPP

#include <QVariant>
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui {	class EditDialog; }
QT_END_NAMESPACE

class EditDialog : public QDialog
{

		Q_OBJECT

	public: struct Item
	{
		QVariant Id;
		QString Name;

		QList<Item> List;
	};

	public: using Items = QList<Item>;

	private:

		Ui::EditDialog *ui;

	public:

		explicit EditDialog(QWidget *parent = nullptr);
		virtual ~EditDialog(void) override;

		QLineEdit* appendEdit(const QString& Name, const QString& Def = QString());
		QSpinBox* appendSpin(const QString& Name, int Def = 0);
		QDoubleSpinBox* appendDoubleSpin(const QString& Name, double Def = 0.0);
		QComboBox* appendCombo(const QString& Name, const QVariantMap& Map, const QVariant& Def = QVariant());
		QList<QComboBox*> appendCombos(const QString& Name, const Items& List, const QVariantList& Def = QVariantList());

		void appendWidget(const QString& Name, QWidget* Widget);
		QWidget* widgetAt(const QString& Name);

};

#endif // EDITDIALOG_HPP
