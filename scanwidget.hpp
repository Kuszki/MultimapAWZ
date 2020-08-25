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

#ifndef SCANWIDGET_HPP
#define SCANWIDGET_HPP

#include <QWidget>
#include <QPixmap>
#include <QFileInfo>
#include <QWheelEvent>
#include <QDesktopServices>

#include "titlewidget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {	class ScanWidget; }
QT_END_NAMESPACE

class ScanWidget : public QWidget
{

		Q_OBJECT

	private:

		QString Path, File, Dir;
		QPixmap Image;

		double Scale = 1.0;
		int Rotation = 0;

		Ui::ScanWidget *ui;

	public:

		explicit ScanWidget(const QString& path = QString(),
						QWidget *parent = nullptr);
		virtual ~ScanWidget(void) override;

		void setTitleWidget(TitleWidget* W);

	public slots:

		void updateImage(const QString& Src);

		void setPath(const QString& P);

		void setStatus(bool Enabled);

	protected:

		virtual void wheelEvent(QWheelEvent* Event) override;

	private slots:

		void zoomInClicked(void);
		void zoomOutClicked(void);
		void zoomOrgClicked(void);
		void zoomFitClicked(void);

		void rotateLeftClicked(void);
		void rotateRightClicked(void);

		void openDirClicked(void);

};

#endif // SCANWIDGET_HPP
