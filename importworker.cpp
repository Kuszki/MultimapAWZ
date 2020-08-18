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

#include "importworker.hpp"

QVariant (*ImportWorker::getID) (QSqlQuery&) = [] (QSqlQuery& q) -> QVariant { return q.exec() && q.next() ? q.value(0) : QVariant(); };

ImportWorker::ImportWorker(QSqlDatabase& Db, QObject *parent)
	: QObject(parent), Database(Db) {}

ImportWorker::~ImportWorker(void) {}

QList<DOKUMENTY> ImportWorker::loadDocs(void)
{
	emit onSubprocessStart(0, 0);

	QList<DOKUMENTY> Dict;
	QSqlQuery Query(Database);

	Query.setForwardOnly(true);
	Query.prepare("SELECT id, nazwa, opis FROM dokumenty");

	if (Query.exec()) while (Query.next())
	{
		Dict.append(
		{
			Query.value(0).toInt(),
			Query.value(1).toString(),
			Query.value(2).toString()
		});
	}

	return Dict;
}

QList<DOK_DZIALKI> ImportWorker::loadDocLot(void)
{
	emit onSubprocessStart(0, 0);

	QList<DOK_DZIALKI> Dict;
	QSqlQuery Query(Database);

	Query.setForwardOnly(true);
	Query.prepare("SELECT id, id_dok, id_dzi FROM dok_dzialki");

	if (Query.exec()) while (Query.next())
	{
		Dict.append(
		{
			Query.value(0).toInt(),
			Query.value(1).toInt(),
			Query.value(2).toInt()
		});
	}

	return Dict;
}

QList<DOK_OSOBY> ImportWorker::loadDocOwn(void)
{
	emit onSubprocessStart(0, 0);

	QList<DOK_OSOBY> Dict;
	QSqlQuery Query(Database);

	Query.setForwardOnly(true);
	Query.prepare("SELECT id, id_dok, id_oso FROM dok_osoby");

	if (Query.exec()) while (Query.next())
	{
		Dict.append(
		{
			Query.value(0).toInt(),
			Query.value(1).toInt(),
			Query.value(2).toInt()
		});
	}

	return Dict;
}

QList<DOK_PLIKI> ImportWorker::loadDocFiles(void)
{
	emit onSubprocessStart(0, 0);

	QList<DOK_PLIKI> Dict;
	QSqlQuery Query(Database);

	Query.setForwardOnly(true);
	Query.prepare("SELECT id, id_dok, id_pli FROM dok_pliki");

	if (Query.exec()) while (Query.next())
	{
		Dict.append(
		{
			Query.value(0).toInt(),
			Query.value(1).toInt(),
			Query.value(2).toInt()
		});
	}

	return Dict;
}

QList<GMINY> ImportWorker::loadComms(void)
{
	emit onSubprocessStart(0, 0);

	QList<GMINY> Dict;
	QSqlQuery Query(Database);

	Query.setForwardOnly(true);
	Query.prepare("SELECT id, nazwa FROM gminy");

	if (Query.exec()) while (Query.next())
	{
		Dict.append(
		{
			Query.value(0).toInt(),
			Query.value(1).toString()
		});
	}

	return Dict;
}

QList<OBREBY> ImportWorker::loadPrecs(void)
{
	emit onSubprocessStart(0, 0);

	QList<OBREBY> Dict;
	QSqlQuery Query(Database);

	Query.setForwardOnly(true);
	Query.prepare("SELECT id, nazwa, id_gminy FROM obreby");

	if (Query.exec()) while (Query.next())
	{
		Dict.append(
		{
			Query.value(0).toInt(),
			Query.value(1).toString(),
			Query.value(2).toInt()
		});
	}

	return Dict;
}

QList<OSOBY> ImportWorker::loadOwns(void)
{
	emit onSubprocessStart(0, 0);

	QList<OSOBY> Dict;
	QSqlQuery Query(Database);

	Query.setForwardOnly(true);
	Query.prepare("SELECT id, imie, nazwisko, im_ojca, im_matki FROM osoby");

	if (Query.exec()) while (Query.next())
	{
		Dict.append(
		{
			Query.value(0).toInt(),
			Query.value(1).toString(),
			Query.value(2).toString(),
			Query.value(3).toString(),
			Query.value(4).toString()
		});
	}

	return Dict;
}

QList<PLIKI> ImportWorker::loadFiles(void)
{
	emit onSubprocessStart(0, 0);

	QList<PLIKI> Dict;
	QSqlQuery Query(Database);

	Query.setForwardOnly(true);
	Query.prepare("SELECT id, nazwa, id_rodzaju, opis FROM pliki");

	if (Query.exec()) while (Query.next())
	{
		Dict.append(
		{
			Query.value(0).toInt(),
			Query.value(1).toString(),
			Query.value(2).toInt(),
			Query.value(3).toString()
		});
	}

	return Dict;
}

QList<DZIALKI> ImportWorker::loadLots(void)
{
	emit onSubprocessStart(0, 0);

	QList<DZIALKI> Dict;
	QSqlQuery Query(Database);

	Query.setForwardOnly(true);
	Query.prepare("SELECT id, numer, arkusz, id_obrebu FROM dokumenty");

	if (Query.exec()) while (Query.next())
	{
		Dict.append(
		{
			Query.value(0).toInt(),
			Query.value(1).toString(),
			Query.value(2).toString(),
			Query.value(3).toInt(),
		});
	}

	return Dict;
}

QList<RODZAJEDOK> ImportWorker::loadRoles(void)
{
	emit onSubprocessStart(0, 0);

	QList<RODZAJEDOK> Dict;
	QSqlQuery Query(Database);

	Query.setForwardOnly(true);
	Query.prepare("SELECT id, nazwa FROM rodzajedok");

	if (Query.exec()) while (Query.next())
	{
		Dict.append(
		{
			Query.value(0).toInt(),
			Query.value(1).toString()
		});
	}

	return Dict;
}

int ImportWorker::appendDocs(const QList<DOKUMENTY>& List)
{
	emit onSubprocessStart(0, List.count()); int Step(0);

	QSqlQuery Query(Database); int Count(0);

	Query.prepare("INSERT INTO dokumenty (id, nazwa, opis) VALUES (?, ?, ?)");

	for (const auto& i : List)
	{
		Query.addBindValue(i.id ? i.id : QVariant());
		Query.addBindValue(i.nazwa.isEmpty() ? QVariant() : i.nazwa);
		Query.addBindValue(i.opis.isEmpty() ? QVariant() : i.opis);

		if (Query.exec()) ++Count;

		emit onSubprocessUpdate(++Step);
	}

	return Count;
}

int ImportWorker::appendDocLot(const QList<DOK_DZIALKI>& List)
{
	emit onSubprocessStart(0, List.count()); int Step(0);

	QSqlQuery Query(Database); int Count(0);

	Query.prepare("INSERT INTO dok_dzialki (id, id_dok, id_dzi) VALUES (?, ?, ?)");

	for (const auto& i : List)
	{
		Query.addBindValue(i.id ? i.id : QVariant());
		Query.addBindValue(i.id_dok ? i.id_dok : QVariant());
		Query.addBindValue(i.id_dzi ? i.id_dzi : QVariant());

		if (Query.exec()) ++Count;

		emit onSubprocessUpdate(++Step);
	}

	return Count;
}

int ImportWorker::appendDocOwn(const QList<DOK_OSOBY>& List)
{
	emit onSubprocessStart(0, List.count()); int Step(0);

	QSqlQuery Query(Database); int Count(0);

	Query.prepare("INSERT INTO dok_osoby (id, id_dok, id_oso) VALUES (?, ?, ?)");

	for (const auto& i : List)
	{
		Query.addBindValue(i.id ? i.id : QVariant());
		Query.addBindValue(i.id_dok ? i.id_dok : QVariant());
		Query.addBindValue(i.id_oso ? i.id_oso : QVariant());

		if (Query.exec()) ++Count;

		emit onSubprocessUpdate(++Step);
	}

	return Count;
}

int ImportWorker::appendDocFiles(const QList<DOK_PLIKI>& List)
{
	emit onSubprocessStart(0, List.count()); int Step(0);

	QSqlQuery Query(Database); int Count(0);

	Query.prepare("INSERT INTO dok_pliki (id, id_dok, id_pli) VALUES (?, ?, ?)");

	for (const auto& i : List)
	{
		Query.addBindValue(i.id ? i.id : QVariant());
		Query.addBindValue(i.id_dok ? i.id_dok : QVariant());
		Query.addBindValue(i.id_pli ? i.id_pli : QVariant());

		if (Query.exec()) ++Count;

		emit onSubprocessUpdate(++Step);
	}

	return Count;
}

int ImportWorker::appendLots(const QList<DZIALKI>& List)
{
	emit onSubprocessStart(0, List.count()); int Step(0);

	QSqlQuery Query(Database); int Count(0);

	Query.prepare("INSERT INTO dzialki (id, numer, arkusz, id_obrebu) VALUES (?, ?, ?, ?)");

	for (const auto& i : List)
	{
		Query.addBindValue(i.id ? i.id : QVariant());
		Query.addBindValue(i.numer.isEmpty() ? QVariant() : i.numer);
		Query.addBindValue(i.arkusz.isEmpty() ? QVariant() : i.arkusz);
		Query.addBindValue(i.id_obrebu ? i.id_obrebu : QVariant());

		if (Query.exec()) ++Count;

		emit onSubprocessUpdate(++Step);
	}

	return Count;
}

int ImportWorker::appendComms(const QList<GMINY>& List)
{
	emit onSubprocessStart(0, List.count()); int Step(0);

	QSqlQuery Query(Database); int Count(0);

	Query.prepare("INSERT INTO gminy (id, nazwa) VALUES (?, ?)");

	for (const auto& i : List)
	{
		Query.addBindValue(i.id ? i.id : QVariant());
		Query.addBindValue(i.nazwa.isEmpty() ? QVariant() : i.nazwa);

		if (Query.exec()) ++Count;

		emit onSubprocessUpdate(++Step);
	}

	return Count;
}

int ImportWorker::appendPrecs(const QList<OBREBY>& List)
{
	emit onSubprocessStart(0, List.count()); int Step(0);

	QSqlQuery Query(Database); int Count(0);

	Query.prepare("INSERT INTO obreby (id, nazwa, id_gminy) VALUES (?, ?, ?)");

	for (const auto& i : List)
	{
		Query.addBindValue(i.id ? i.id : QVariant());
		Query.addBindValue(i.nazwa.isEmpty() ? QVariant() : i.nazwa);
		Query.addBindValue(i.id_gminy ? i.id_gminy : QVariant());

		if (Query.exec()) ++Count;

		emit onSubprocessUpdate(++Step);
	}

	return Count;
}

int ImportWorker::appendOwns(const QList<OSOBY>& List)
{
	emit onSubprocessStart(0, List.count()); int Step(0);

	QSqlQuery Query(Database); int Count(0);

	Query.prepare("INSERT INTO osoby (id, imie, nazwisko, im_ojca, im_matki) VALUES (?, ?, ?, ?, ?)");

	for (const auto& i : List)
	{
		Query.addBindValue(i.id ? i.id : QVariant());
		Query.addBindValue(i.imie.isEmpty() ? QVariant() : i.imie);
		Query.addBindValue(i.nazwisko.isEmpty() ? QVariant() : i.nazwisko);
		Query.addBindValue(i.im_ojca.isEmpty() ? QVariant() : i.im_ojca);
		Query.addBindValue(i.im_matki.isEmpty() ? QVariant() : i.im_matki);

		if (Query.exec()) ++Count;

		emit onSubprocessUpdate(++Step);
	}

	return Count;
}

int ImportWorker::appendFiles(const QList<PLIKI>& List)
{
	emit onSubprocessStart(0, List.count()); int Step(0);

	QSqlQuery Query(Database); int Count(0);

	Query.prepare("INSERT INTO pliki (id, nazwa, id_rodzaju, opis) VALUES (?, ?, ?)");

	for (const auto& i : List)
	{
		Query.addBindValue(i.id ? i.id : QVariant());
		Query.addBindValue(i.nazwa.isEmpty() ? QVariant() : i.nazwa);
		Query.addBindValue(i.id_rodzaju ? i.id_rodzaju : QVariant());
		Query.addBindValue(i.opis.isEmpty() ? QVariant() : i.opis);

		if (Query.exec()) ++Count;

		emit onSubprocessUpdate(++Step);
	}

	return Count;
}

int ImportWorker::appendRoles(const QList<RODZAJEDOK>& List)
{
	emit onSubprocessStart(0, List.count()); int Step(0);

	QSqlQuery Query(Database); int Count(0);

	Query.prepare("INSERT INTO rodzajedok (id, nazwa) VALUES (?, ?)");

	for (const auto& i : List)
	{
		Query.addBindValue(i.id ? i.id : QVariant());
		Query.addBindValue(i.nazwa.isEmpty() ? QVariant() : i.nazwa);

		if (Query.exec()) ++Count;

		emit onSubprocessUpdate(++Step);
	}

	return Count;
}

int ImportWorker::importSheets(const QString& Path, const QMap<ROLES, int>& Roles, const QString& Fsep, const QString& Lsep)
{
	emit onActionStart(tr("Preparing queries"));
	emit onProgressStart(0, 6);

	QSqlQuery genDocId(Database); genDocId.prepare("SELECT GEN_ID(GEN_DOKUMENTY_ID, 1) FROM RDB$DATABASE");
	QSqlQuery genLotId(Database); genLotId.prepare("SELECT GEN_ID(GEN_DZIALKI_ID, 1) FROM RDB$DATABASE");
	QSqlQuery genOwnId(Database); genOwnId.prepare("SELECT GEN_ID(GEN_OSOBY_ID, 1) FROM RDB$DATABASE");
	QSqlQuery genCommId(Database); genCommId.prepare("SELECT GEN_ID(GEN_GMINY_ID, 1) FROM RDB$DATABASE");
	QSqlQuery genPrecId(Database); genPrecId.prepare("SELECT GEN_ID(GEN_OBREBY_ID, 1) FROM RDB$DATABASE");

	QSqlQuery genDocLotId(Database); genDocLotId.prepare("SELECT GEN_ID(GEN_DOK_DZIALKI_ID, 1) FROM RDB$DATABASE");
	QSqlQuery genDocOwnId(Database); genDocOwnId.prepare("SELECT GEN_ID(GEN_DOK_OSOBY_ID, 1) FROM RDB$DATABASE");

	QList<DOKUMENTY> addDocs; QList<DZIALKI> addLots; QList<OSOBY> addOwns;
	QList<DOK_DZIALKI> addDocLot; QList<DOK_OSOBY> addDocOwn;

	auto oldDocs = loadDocs();
	auto oldLots = loadLots();
	auto oldOwns = loadOwns();

	auto oldDocLot = loadDocLot();
	auto oldDocOwn = loadDocOwn();

	const auto dictComms = loadComms();
	const auto dictPrecs = loadPrecs();

	QFile File(Path);
	File.open(QFile::ReadOnly | QFile::Text);

	// FIXME: add return if fail

	const QRegExp fExp(QString("\\s*%1\\s*").arg(Fsep));
	const QRegExp lExp(QString("\\s*%1\\s*").arg(Lsep));

	while (!File.atEnd())
	{
		const QStringList List = QString::fromLocal8Bit(File.readLine()).split(fExp);
		const QStringList Lots = List.value(Roles[ROLES::DZIALKI]).split(lExp);

		int commID(0), precID(0), owCol(Roles[ROLES::OSOBY]);

		DOKUMENTY Dk = { 0, List[Roles[ROLES::NUMER]], List[Roles[ROLES::UWAGI]] };

		const auto isDoc = hasItemByField(oldDocs, Dk.nazwa, &DOKUMENTY::nazwa) ?
			getItemByField(oldDocs, Dk.nazwa, &DOKUMENTY::nazwa).id : 0;

		if (isDoc) Dk.id = isDoc;
		else
		{
			Dk.id = getID(genDocId).toInt();
			addDocs.append(Dk);
			oldDocs.append(Dk);
		}

		commID = hasItemByField(dictComms, List[Roles[ROLES::GMINA]], &GMINY::nazwa) ?
			getItemByField(dictComms, List[Roles[ROLES::GMINA]], &GMINY::nazwa).id : 0;

		for (const auto& i : dictPrecs)
			if (i.id_gminy == commID && i.nazwa == List[Roles[ROLES::OBREB]])
				precID = i.id;

		for (const auto& i : Lots)
		{
			const QStringList Data = i.split('-');
			const QString Nr = Data.size() == 2 ? Data[1] : Data[0];
			const QString Ak = Data.size() == 2 ? Data[0] : QString();

			DZIALKI Dz = { 0, Nr, Ak, precID };

			for (const auto& l : oldLots) if (Dz.numer == l.numer &&
									    Dz.arkusz == l.arkusz &&
									    Dz.id_obrebu == l.id_obrebu)
				Dz.id = l.id;

			if (!Dz.id)
			{
				Dz.id = getID(genLotId).toInt();
				addLots.append(Dz);
				oldLots.append(Dz);
			}

			DOK_DZIALKI Dd = { 0, Dk.id, Dz.id };

			for (const auto& l : oldDocLot) if (Dd.id_dok == l.id_dok &&
										 Dd.id_dzi == l.id_dzi)
				Dd.id = l.id;

			if (!Dd.id)
			{
				Dd.id = getID(genDocLotId).toInt();
				addDocLot.append(Dd);
				oldDocLot.append(Dd);
			}
		}

		while (owCol + 4 < List.size())
		{
			OSOBY Os =
			{
				0,
				List[owCol++],
				List[owCol++],
				List[owCol++],
				List[owCol++]
			};

			for (const auto& l : oldOwns) if (Os.imie == l.imie &&
									    Os.nazwisko == l.nazwisko &&
									    Os.im_ojca == l.im_ojca &&
									    Os.im_matki == l.im_matki)
				Os.id = l.id;

			if (!Os.id)
			{
				Os.id = getID(genOwnId).toInt();
				addOwns.append(Os);
				oldOwns.append(Os);
			}

			DOK_OSOBY Do = { 0, Dk.id, Os.id };

			for (const auto& l : oldDocOwn) if (Do.id_dok == l.id_dok &&
										 Do.id_oso == l.id_oso)
				Do.id = l.id;

			if (!Do.id)
			{
				Do.id = getID(genDocOwnId).toInt();
				addDocOwn.append(Do);
				oldDocOwn.append(Do);
			}
		}
	}

	emit onProgressUpdate(1);
	emit onActionStart(tr("Uploading documents"));
	int Count = appendDocs(addDocs);

	emit onProgressUpdate(2);
	emit onActionStart(tr("Uploading lots"));
	appendLots(addLots);

	emit onProgressUpdate(3);
	emit onActionStart(tr("Uploading owners"));
	appendOwns(addOwns);

	emit onProgressUpdate(4);
	emit onActionStart(tr("Uploading relations"));
	appendDocLot(addDocLot);

	emit onProgressUpdate(5);
	emit onActionStart(tr("Uploading relations"));
	appendDocOwn(addDocOwn);

	return Count;
}

int ImportWorker::importScans(const QString& Path, const QString& Fsep)
{
	emit onActionStart(tr("Preparing queries"));
	emit onProgressStart(0, 3);

	QSqlQuery genFileId(Database); genFileId.prepare("SELECT GEN_ID(GEN_PLIKI_ID, 1) FROM RDB$DATABASE");

	QSqlQuery genDocFilId(Database); genDocFilId.prepare("SELECT GEN_ID(GEN_DOK_PLIKI_ID, 1) FROM RDB$DATABASE");

	QList<PLIKI> addFiles;
	QList<DOK_PLIKI> addDocFil;

	auto oldFiles = loadFiles();

	auto oldDocFil = loadDocFiles();

	const auto dictDocs = loadDocs();
	const auto dictRoles = loadRoles();

	QFile File(Path);
	File.open(QFile::ReadOnly | QFile::Text);

	const QRegExp fExp(QString("\\s*%1\\s*").arg(Fsep));

	while (!File.atEnd())
	{
		const QStringList List = QString::fromLocal8Bit(File.readLine()).split(fExp);

		const auto Doc = hasItemByField(dictDocs, List[0], &DOKUMENTY::nazwa) ?
			getItemByField(dictDocs, List[0], &DOKUMENTY::nazwa).id : 0;

		const auto filID = hasItemByField(oldFiles, List[1], &PLIKI::nazwa) ?
			getItemByField(oldFiles, List[1], &PLIKI::nazwa).id : 0;

		PLIKI Pl = { filID, List[1], Doc };

		if (!Pl.id)
		{
			Pl.id = getID(genFileId).toInt();
			addFiles.append(Pl);
			oldFiles.append(Pl);
		}

		DOK_PLIKI Dp = { 0, Doc, Pl.id };

		for (const auto& l : oldDocFil) if (Dp.id_dok == l.id_dok &&
									 Dp.id_pli == l.id_pli)
			Dp.id = l.id;

		if (!Dp.id)
		{
			Dp.id = getID(genDocFilId).toInt();
			addDocFil.append(Dp);
			oldDocFil.append(Dp);
		}
	}

	emit onProgressUpdate(1);
	emit onActionStart(tr("Uploading files"));
	int Count = appendFiles(addFiles);

	emit onProgressUpdate(2);
	emit onActionStart(tr("Uploading relations"));
	appendDocFiles(addDocFil);

	return Count;
}

int ImportWorker::importDict(const QString& Path, const QString& Fsep)
{
	emit onActionStart(tr("Preparing queries"));
	emit onProgressStart(0, 2);

	QList<RODZAJEDOK> addDict;

	QFile File(Path);
	File.open(QFile::ReadOnly | QFile::Text);

	const QRegExp fExp(QString("\\s*%1\\s*").arg(Fsep));

	while (!File.atEnd())
	{
		const QStringList List = QString::fromLocal8Bit(File.readLine()).split(fExp);

		addDict.append({ List[0].toInt(), List[1] });
	}

	emit onProgressUpdate(1);
	emit onActionStart(tr("Uploading dictionary"));
	int Count = appendRoles(addDict);

	return Count;
}

void ImportWorker::importDocuments(const QString& DocPath, const QString& FilesPath, const QString& DictPath, const QMap<ROLES, int>& Roles, const QString& Fsep, const QString& Lsep)
{

}
