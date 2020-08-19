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

#ifndef COMMONH_HPP
#define COMMONH_HPP

#include <QtCore>

enum class ROLES
{
	ID,
	PLIK,
	GMINA,
	OBREB,
	NUMER,
	DZIALKI,
	UWAGI,
	OSOBY,
	ROLA
};

struct DOKUMENTY
{
	qint32 id;
	QString nazwa;
	QString opis;
};

struct DOK_DZIALKI
{
	qint32 id;
	qint32 id_dok;
	qint32 id_dzi;
};

struct DOK_OSOBY
{
	qint32 id;
	qint32 id_dok;
	qint32 id_oso;
};

struct DOK_PLIKI
{
	qint32 id;
	qint32 id_dok;
	qint32 id_pli;
};

struct DZIALKI
{
	qint32 id;
	QString numer;
	QString arkusz;
	qint32 id_obrebu;
};

struct GMINY
{
	qint32 id;
	QString nazwa;
};

struct OBREBY
{
	qint32 id;
	QString nazwa;
	qint32 id_gminy;
};

struct OSOBY
{
	qint32 id;
	QString imie;
	QString nazwisko;
	QString im_ojca;
	QString im_matki;
};

struct PLIKI
{
	qint32 id;
	QString nazwa;
	qint32 id_rodzaju;
	QString opis;
};

struct RODZAJEDOK
{
	qint32 id;
	QString nazwa;
};

template<class Type, class Field, template<class> class Container>
Type& getItemByField(Container<Type>& Items, const Field& Data, Field Type::*Pointer)
{
	for (auto& Item : Items) if (Item.*Pointer == Data) return Item;
}

template<class Type, class Field, template<class> class Container>
const Type& getItemByField(const Container<Type>& Items, const Field& Data, Field Type::*Pointer)
{
	for (auto& Item : Items) if (Item.*Pointer == Data) return Item;
}

template<class Type, class Field, template<class> class Container>
bool hasItemByField(const Container<Type>& Items, const Field& Data, Field Type::*Pointer)
{
	for (auto& Item : Items) if (Item.*Pointer == Data) return true; return false;
}

#endif // COMMONH_HPP
