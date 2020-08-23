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
#ifndef IMPORTWORKER_HPP
#define IMPORTWORKER_HPP

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QObject>
#include <QVariant>
#include <QHash>
#include <QMap>
#include <QList>

#include "commonh.hpp"

class ImportWorker : public QObject
{

		Q_OBJECT

	public: enum TabName
		{
			Doc,
			Doc_Lot,
			Doc_Own,
			Doc_Fil,
			Lot,
			Com,
			Pre,
			Own,
			Fil,
			Fil_Rol
		};

	private:

		static QVariant (*getID) (QSqlQuery&);

		QSqlDatabase& Database;

	public:

		explicit ImportWorker(QSqlDatabase& Db, QObject *parent = nullptr);
		virtual ~ImportWorker(void) override;

		QList<DOKUMENTY> loadDocs(void);
		QList<DOK_DZIALKI> loadDocLot(void);
		QList<DOK_OSOBY> loadDocOwn(void);
		QList<DOK_PLIKI> loadDocFiles(void);
		QList<DZIALKI> loadLots(void);
		QList<GMINY> loadComms(void);
		QList<OBREBY> loadPrecs(void);
		QList<OSOBY> loadOwns(void);
		QList<PLIKI> loadFiles(void);
		QList<RODZAJEDOK> loadRoles(void);

		int findDocs(const DOKUMENTY& List);
		int findDocLots(const DOK_DZIALKI& List);
		int findDocOwns(const DOK_OSOBY& List);
		int findDocFiles(const DOK_PLIKI& List);
		int findLots(const DZIALKI& List);
		int findComms(const GMINY& List);
		int findPrecs(const OBREBY& List);
		int findOwns(const OSOBY& List);
		int findFiles(const PLIKI& List);
		int findRoles(const RODZAJEDOK& List);

		int appendDocs(const QList<DOKUMENTY>& List);
		int appendDocLot(const QList<DOK_DZIALKI>& List);
		int appendDocOwn(const QList<DOK_OSOBY>& List);
		int appendDocFiles(const QList<DOK_PLIKI>& List);
		int appendLots(const QList<DZIALKI>& List);
		int appendComms(const QList<GMINY>& List);
		int appendPrecs(const QList<OBREBY>& List);
		int appendOwns(const QList<OSOBY>& List);
		int appendFiles(const QList<PLIKI>& List);
		int appendRoles(const QList<RODZAJEDOK>& List);

		int removeDocs(const QSet<int>& List);
		int removeLots(const QSet<int>& List);
		int removeComms(const QSet<int>& List);
		int removePrecs(const QSet<int>& List);
		int removeOwns(const QSet<int>& List);
		int removeFiles(const QSet<int>& List);
		int removeRoles(const QSet<int>& List);

		int removeDocLots(const QSet<int>& List, bool Rel = false);
		int removeDocOwns(const QSet<int>& List, bool Rel = false);
		int removeDocFiles(const QSet<int>& List, bool Rel = false);

		bool removeDocLot(int dID, int fID);
		bool removeDocOwn(int dID, int fID);
		bool removeDocFile(int dID, int fID);

		int generateId(TabName Tab);

	public slots:

		int importSheets(const QString& Path,
					  const QMap<ROLES, int>& Roles,
					  const QString& Fsep = ";",
					  const QString& Lsep = ",");

		int importScans(const QString& Path,
					 const QMap<ROLES, int>& Roles,
					 const QString& Fsep = ";");

		int importDict(const QString& Path,
					const QMap<ROLES, int>& Roles,
					const QString& Fsep = ";");

	signals:

		void onActionStart(const QString&);

		void onProgressStart(int, int);
		void onProgressUpdate(int);

		void onSubprocessStart(int, int);
		void onSubprocessUpdate(int);

		void onJobEnd(const QString&);

};

#endif // IMPORTWORKER_HPP
