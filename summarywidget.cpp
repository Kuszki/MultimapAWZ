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

#include "summarywidget.hpp"
#include "ui_summarywidget.h"

SummaryWidget::SummaryWidget(QSqlDatabase& Db, QWidget *parent)
: QWidget(parent), Database(Db), ui(new Ui::SummaryWidget)
{
	ui->setupUi(this);
}

SummaryWidget::~SummaryWidget(void)
{
	delete ui;
}

void SummaryWidget::setTitleWidget(TitleWidget* W)
{
	file = new QToolButton(this);
	file->setText(tr("Open document"));
	file->setIcon(QIcon::fromTheme("document-new"));
	file->setEnabled(false);

	W->addRightWidget(file);

	dir = new QToolButton(this);
	dir->setText(tr("Open folder"));
	dir->setIcon(QIcon::fromTheme("folder-new"));
	dir->setEnabled(false);

	W->addRightWidget(dir);

	connect(file, &QToolButton::clicked, this, &SummaryWidget::fileButtonClicked);
	connect(dir, &QToolButton::clicked, this, &SummaryWidget::dirButtonClicked);
}

void SummaryWidget::updateImage(const QString& Src)
{
	File = Path + "/" + Src;
	Dir = QFileInfo(File).path();

	file->setEnabled(QFile(File).exists());
	dir->setEnabled(QDir(Dir).exists());
}

void SummaryWidget::setPath(const QString& P)
{
	Path = P;
}

void SummaryWidget::filterList(int ID)
{
	lastID = ID;
	reloadList();
}

void SummaryWidget::reloadList(void)
{
	ui->ownLabel->clear();
	ui->lotLabel->clear();
	ui->fileLabel->clear();

	if (lastID == -1) return;

	QSqlQuery Query(Database);
	QHash<QString, QStringList> Ll;
	QStringList O, L, F;

	Query.prepare("SELECT o.imie, o.nazwisko, o.im_ojca, o.im_matki FROM osoby o "
			    "INNER JOIN dok_osoby j ON j.id_oso = o.id "
			    "WHERE j.id_dok = ? "
			    "ORDER BY o.nazwisko ASC, o.imie ASC");
	Query.addBindValue(lastID);

	if (Query.exec()) while (Query.next())
	{
		O.append(QString("* %1 %2 (%3, %4)")
			    .arg(Query.value(0).toString())
			    .arg(Query.value(1).toString())
			    .arg(Query.value(2).toString())
			    .arg(Query.value(3).toString()));
	}

	Query.prepare("SELECT COALESCE(d.arkusz || '-' || d.numer, d.numer), g.nazwa || '/' || o.nazwa FROM dzialki d "
			    "INNER JOIN dok_dzialki j ON j.id_dzi = d.id "
			    "INNER JOIN obreby o ON d.id_obrebu = o.id "
			    "INNER JOIN gminy g ON o.id_gminy = g.id "
			    "WHERE j.id_dok = ? "
			    "ORDER BY g.nazwa ASC, o.nazwa ASC, d.arkusz ASC, d.numer ASC");
	Query.addBindValue(lastID);

	if (Query.exec()) while (Query.next())
	{
		Ll[Query.value(1).toString()].append(Query.value(0).toString());
	}

	Query.prepare("SELECT r.nazwa, o.katalog || '/' || o.nazwa, '(' || o.opis || ')' FROM pliki o "
			    "INNER JOIN dok_pliki j ON j.id_pli = o.id "
			    "INNER JOIN rodzajedok r ON o.id_rodzaju = r.id "
			    "WHERE j.id_dok = ? "
			    "ORDER BY r.id ASC, o.nazwa ASC");
	Query.addBindValue(lastID);

	if (Query.exec()) while (Query.next())
	{
		F.append(QString("* %1 - %2 %3")
			    .arg(Query.value(0).toString())
			    .arg(Query.value(1).toString())
			    .arg(Query.value(2).toString()));
	}

	for (auto i = Ll.constBegin(); i != Ll.constEnd(); ++i)
	{
		L.append(QString("* %1: %2").arg(i.key()).arg(i.value().join(", ")));
	}

	ui->ownLabel->setTextFormat(Qt::MarkdownText);
	ui->ownLabel->setText(O.join('\n'));

	ui->lotLabel->setTextFormat(Qt::MarkdownText);
	ui->lotLabel->setText(L.join('\n'));

	ui->fileLabel->setTextFormat(Qt::MarkdownText);
	ui->fileLabel->setText(F.join('\n'));
}

void SummaryWidget::setStatus(bool Enabled)
{
	setEnabled(Enabled);
}

void SummaryWidget::dirButtonClicked(void)
{
	QDesktopServices::openUrl(QUrl::fromLocalFile(Dir));
}

void SummaryWidget::fileButtonClicked(void)
{
	QDesktopServices::openUrl(QUrl::fromLocalFile(File));
}
