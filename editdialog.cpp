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

#include "editdialog.hpp"
#include "ui_editdialog.h"

EditDialog::EditDialog(QWidget *parent)
	: QDialog(parent), ui(new Ui::EditDialog)
{
	ui->setupUi(this);

	ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
}

EditDialog::~EditDialog(void)
{
	delete ui;
}

std::function<bool (const QVariantMap&)> EditDialog::getValidator(void) const
{
	return Validator;
}

void EditDialog::setValidator(const std::function<bool (const QVariantMap&)>& value)
{
	Validator = value;
}

QLineEdit* EditDialog::appendEdit(const QString& Name, const QString& Def)
{
	auto Widget = new QLineEdit(Def);

	appendWidget(Name, Widget);

	return Widget;
}

QSpinBox* EditDialog::appendSpin(const QString& Name, int Def, int Min, int Max)
{
	auto Widget = new QSpinBox();

	Widget->setRange(Min, Max);
	Widget->setValue(Def);

	appendWidget(Name, Widget);

	return Widget;
}

QDoubleSpinBox* EditDialog::appendDoubleSpin(const QString& Name, double Def, double Min, double Max)
{
	auto Widget = new QDoubleSpinBox();

	Widget->setRange(Min, Max);
	Widget->setValue(Def);

	appendWidget(Name, Widget);

	return Widget;
}

QComboBox* EditDialog::appendCombo(const QString& Name, const QVariantMap& Map, const QVariant& Def)
{
	auto Widget = new QComboBox();

	for (auto i = Map.constBegin(); i != Map.constEnd(); ++i)
	{
		Widget->addItem(i.key(), i.value());
	}

	if (!Def.isNull())
	{
		if (Widget->findText(Def.toString()) != -1)
			Widget->setCurrentText(Def.toString());
		else
			Widget->setCurrentIndex(Widget->findData(Def));
	}

	appendWidget(Name, Widget);

	return Widget;
}

QDateEdit* EditDialog::appendDate(const QString& Name, const QDate& Def)
{
	auto Widget = new QDateEdit(Def);

	appendWidget(Name, Widget);

	return Widget;
}

QDateTimeEdit* EditDialog::appendDateTime(const QString& Name, const QDateTime& Def)
{
	auto Widget = new QDateTimeEdit(Def);

	appendWidget(Name, Widget);

	return Widget;
}

void EditDialog::appendWidget(const QString& Name, QWidget* Widget)
{
	const int Row = ui->gridLayout->rowCount();

	Widget->setObjectName(Name);
	Widget->setParent(this);

	Widgets.append(Widget);

	ui->gridLayout->addWidget(new QLabel(Name, this), Row, 0);
	ui->gridLayout->addWidget(Widget, Row, 1);

	if (auto W = dynamic_cast<QComboBox*>(Widget))
	{
		connect(W, &QComboBox::currentTextChanged,
			   this, &EditDialog::validateData);
	}
	else if (auto W = dynamic_cast<QLineEdit*>(Widget))
	{
		connect(W, &QLineEdit::textChanged,
			   this, &EditDialog::validateData);
	}
	else if (auto W = dynamic_cast<QSpinBox*>(Widget))
	{
		connect(W, qOverload<int>(&QSpinBox::valueChanged),
			   this, &EditDialog::validateData);
	}
	else if (auto W = dynamic_cast<QDoubleSpinBox*>(Widget))
	{
		connect(W, qOverload<double>(&QDoubleSpinBox::valueChanged),
			   this, &EditDialog::validateData);
	}
	else if (auto W = dynamic_cast<QDateEdit*>(Widget))
	{
		connect(W, &QDateEdit::dateChanged,
			   this, &EditDialog::validateData);
	}
	else if (auto W = dynamic_cast<QDateTimeEdit*>(Widget))
	{
		connect(W, &QDateTimeEdit::dateTimeChanged,
			   this, &EditDialog::validateData);
	}
}

QVariant EditDialog::data(const QString& Name)
{
	QWidget* Widget = nullptr;

	for (auto& i : Widgets) if (i->objectName() == Name) Widget = i;

	if (!Widget) return QVariant();

	if (auto W = dynamic_cast<QComboBox*>(Widget))
	{
		const auto Text = W->currentText();
		const int Index = W->findText(Text);

		if (Index == -1) return Text;
		else return W->itemData(Index);
	}
	else if (auto W = dynamic_cast<QLineEdit*>(Widget))
	{
		return W->text().trimmed().replace("'", "''");
	}
	else if (auto W = dynamic_cast<QSpinBox*>(Widget))
	{
		return W->value();
	}
	else if (auto W = dynamic_cast<QDoubleSpinBox*>(Widget))
	{
		return W->value();
	}
	else if (auto W = dynamic_cast<QDateEdit*>(Widget))
	{
		return W->date();
	}
	else if (auto W = dynamic_cast<QDateTimeEdit*>(Widget))
	{
		return W->dateTime();
	}
	else return QVariant();
}

QVariantMap EditDialog::data(void)
{
	QVariantMap Map;

	for (auto& Widget : Widgets)
	{
		const QString Name = Widget->objectName();

		if (auto W = dynamic_cast<QComboBox*>(Widget))
		{
			Map.insert(Name, W->currentData());
		}
		else if (auto W = dynamic_cast<QLineEdit*>(Widget))
		{
			Map.insert(Name, W->text().trimmed().replace("'", "''"));
		}
		else if (auto W = dynamic_cast<QSpinBox*>(Widget))
		{
			Map.insert(Name, W->value());
		}
		else if (auto W = dynamic_cast<QDoubleSpinBox*>(Widget))
		{
			Map.insert(Name, W->value());
		}
		else if (auto W = dynamic_cast<QDateEdit*>(Widget))
		{
			Map.insert(Name, W->date());
		}
		else if (auto W = dynamic_cast<QDateTimeEdit*>(Widget))
		{
			Map.insert(Name, W->dateTime());
		}
	}

	return Map;
}

void EditDialog::accept(void)
{
	QDialog::accept(); emit onAccept(data());
}

void EditDialog::validateData(void)
{
	ui->buttonBox->button(QDialogButtonBox::Save)
		->setEnabled(!Validator || Validator(data()));
}
