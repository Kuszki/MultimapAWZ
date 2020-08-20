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

#include "scanwidget.hpp"
#include "ui_scanwidget.h"

ScanWidget::ScanWidget(const QString& path, QWidget *parent)
: QWidget(parent), Path(path), ui(new Ui::ScanWidget)
{
	ui->setupUi(this);

	ui->fitButton->setEnabled(false);
	ui->orgButton->setEnabled(false);
	ui->zominButton->setEnabled(false);
	ui->zomoutButton->setEnabled(false);
	ui->rotrightButton->setEnabled(false);
	ui->rotleftButton->setEnabled(false);
	ui->printButton->setEnabled(false);
	ui->opendirButton->setEnabled(false);
	ui->saveButton->setEnabled(false);
}

ScanWidget::~ScanWidget(void)
{
	delete ui;
}

void ScanWidget::setTitleWidget(TitleWidget* W)
{
	ui->horizontalLayout->removeWidget(ui->fitButton);
	W->addRightWidget(ui->fitButton);

	ui->horizontalLayout->removeWidget(ui->orgButton);
	W->addRightWidget(ui->orgButton);

	ui->horizontalLayout->removeWidget(ui->zominButton);
	W->addRightWidget(ui->zominButton);

	ui->horizontalLayout->removeWidget(ui->zomoutButton);
	W->addRightWidget(ui->zomoutButton);

	ui->horizontalLayout->removeWidget(ui->rotrightButton);
	W->addRightWidget(ui->rotrightButton);

	ui->horizontalLayout->removeWidget(ui->rotleftButton);
	W->addRightWidget(ui->rotleftButton);

	ui->horizontalLayout->removeWidget(ui->printButton);
	W->addRightWidget(ui->printButton);

	ui->horizontalLayout->removeWidget(ui->opendirButton);
	W->addRightWidget(ui->opendirButton);

	ui->horizontalLayout->removeWidget(ui->saveButton);
	W->addRightWidget(ui->saveButton);

	ui->horizontalLayout->deleteLater();
}

void ScanWidget::updateImage(const QString& Src)
{
	const QString File = Path + "/" + Src;
	const bool Img = QFile(File).exists();

	if (Img)
	{
		Image = QPixmap(File);
		Rotation = 0;

		ui->label->clear();
		zoomFitClicked();
	}
	else
	{
		Image = QPixmap();

		ui->label->clear();
		ui->label->setText(tr("Select document"));
	}

	ui->fitButton->setEnabled(Img);
	ui->orgButton->setEnabled(Img);
	ui->zominButton->setEnabled(Img);
	ui->zomoutButton->setEnabled(Img);
	ui->rotrightButton->setEnabled(Img);
	ui->rotleftButton->setEnabled(Img);
	ui->printButton->setEnabled(Img);
	ui->opendirButton->setEnabled(Img);
}

void ScanWidget::setPath(const QString& P)
{
	Path = P;
}

void ScanWidget::setStatus(bool Enabled)
{
	ui->fitButton->setEnabled(false);
	ui->orgButton->setEnabled(false);
	ui->zominButton->setEnabled(false);
	ui->zomoutButton->setEnabled(false);
	ui->rotrightButton->setEnabled(false);
	ui->rotleftButton->setEnabled(false);
	ui->printButton->setEnabled(false);
	ui->opendirButton->setEnabled(false);
	ui->saveButton->setEnabled(false);

	if (Enabled) ui->label->setText(tr("Select document to preview"));
	else
	{
		ui->label->clear();

		Image = QPixmap();
		Scale = 1.0;
		Rotation = 0;
	}
}

void ScanWidget::wheelEvent(QWheelEvent* Event)
{
	QWidget::wheelEvent(Event);

	if (!Image.isNull() && QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
	{
		if (Event->angleDelta().y() > 0) zoomInClicked();
		else if (Event->angleDelta().y() < 0) zoomOutClicked();
	}
}

void ScanWidget::zoomInClicked(void)
{
	ui->label->setPixmap(Image.scaledToHeight(int((Scale += 0.1) * Image.height()))
					 .transformed(QTransform().rotate(Rotation)));
}

void ScanWidget::zoomOutClicked(void)
{
	ui->label->setPixmap(Image.scaledToHeight(int((Scale -= 0.1) * Image.height()))
					 .transformed(QTransform().rotate(Rotation)));
}

void ScanWidget::zoomOrgClicked(void)
{
	ui->label->setPixmap(Image.transformed(QTransform().rotate(Rotation)));

	Scale = 1.0;
}

void ScanWidget::zoomFitClicked(void)
{
	auto Img = Image.transformed(QTransform().rotate(Rotation))
			 .scaled(ui->scrollArea->size(), Qt::KeepAspectRatio);

	Scale = double(Img.height()) / double(Image.height());

	ui->label->setPixmap(Img);
}

void ScanWidget::rotateLeftClicked(void)
{
	if ((Rotation -= 90) <= -360) Rotation = 0;

	ui->label->setPixmap(Image.scaledToHeight(int(Scale* Image.height()))
					 .transformed(QTransform().rotate(Rotation)));

	ui->saveButton->setEnabled(Rotation);
}

void ScanWidget::rotateRightClicked(void)
{
	if ((Rotation += 90) >= 360) Rotation = 0;

	ui->label->setPixmap(Image.scaledToHeight(int(Scale* Image.height()))
					 .transformed(QTransform().rotate(Rotation)));

	ui->saveButton->setEnabled(Rotation);
}

void ScanWidget::saveRotClicked(void)
{
	// TODO implement me

	auto Img = Image.transformed(QTransform().rotate(Rotation));
}

void ScanWidget::printDocClicked(void)
{
	// TODO implement me
}

void ScanWidget::openDirClicked(void)
{
	// TODO implement me
}
