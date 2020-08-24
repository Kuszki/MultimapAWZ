/****************** SEQUENCES ********************/

CREATE SEQUENCE GEN_DOKUMENTY_ID;
CREATE SEQUENCE GEN_DOK_DZIALKI_ID;
CREATE SEQUENCE GEN_DOK_OSOBY_ID;
CREATE SEQUENCE GEN_DOK_PLIKI_ID;
CREATE SEQUENCE GEN_DZIALKI_ID;
CREATE SEQUENCE GEN_GMINY_ID;
CREATE SEQUENCE GEN_OBREBY_ID;
CREATE SEQUENCE GEN_OSOBY_ID;
CREATE SEQUENCE GEN_PLIKI_ID;
CREATE SEQUENCE GEN_RODZAJEDOK_ID;

/******************** TABLES **********************/

CREATE TABLE DOKUMENTY
(
  ID INTEGER NOT NULL,
  NAZWA VARCHAR(256) NOT NULL,
  OPIS VARCHAR(1024) DEFAULT null,
  CONSTRAINT PK_DOKUMENTY_0 PRIMARY KEY (ID),
  CONSTRAINT UNQ_DOKUMENTY_0 UNIQUE (NAZWA)
);
CREATE TABLE DOK_DZIALKI
(
  ID INTEGER NOT NULL,
  ID_DOK INTEGER NOT NULL,
  ID_DZI INTEGER NOT NULL,
  CONSTRAINT PK_DOK_DZIALKI_0 PRIMARY KEY (ID),
  CONSTRAINT UNQ_DOK_DZIALKI_0 UNIQUE (ID_DOK,ID_DZI)
);
CREATE TABLE DOK_OSOBY
(
  ID INTEGER NOT NULL,
  ID_DOK INTEGER NOT NULL,
  ID_OSO INTEGER NOT NULL,
  CONSTRAINT PK_DOK_OSOBY_0 PRIMARY KEY (ID),
  CONSTRAINT UNQ_DOK_OSOBY_0 UNIQUE (ID_DOK,ID_OSO)
);
CREATE TABLE DOK_PLIKI
(
  ID INTEGER NOT NULL,
  ID_DOK INTEGER NOT NULL,
  ID_PLI INTEGER NOT NULL,
  CONSTRAINT PK_DOK_PLIKI_0 PRIMARY KEY (ID),
  CONSTRAINT UNQ_DOK_PLIKI_0 UNIQUE (ID_DOK,ID_PLI)
);
CREATE TABLE DZIALKI
(
  ID INTEGER NOT NULL,
  NUMER VARCHAR(64) NOT NULL,
  ARKUSZ VARCHAR(64) DEFAULT null,
  ID_OBREBU INTEGER NOT NULL,
  CONSTRAINT PK_DZIALKI_0 PRIMARY KEY (ID),
  CONSTRAINT UNQ_DZIALKI_0 UNIQUE (NUMER,ARKUSZ,ID_OBREBU)
);
CREATE TABLE GMINY
(
  ID INTEGER NOT NULL,
  NAZWA VARCHAR(128) NOT NULL,
  CONSTRAINT PK_GMINY_0 PRIMARY KEY (ID),
  CONSTRAINT UNQ_GMINY_0 UNIQUE (NAZWA)
);
CREATE TABLE OBREBY
(
  ID INTEGER NOT NULL,
  NAZWA VARCHAR(128) NOT NULL,
  ID_GMINY INTEGER NOT NULL,
  CONSTRAINT PK_OBREBY_0 PRIMARY KEY (ID),
  CONSTRAINT UNQ_OBREBY_0 UNIQUE (NAZWA,ID_GMINY)
);
CREATE TABLE OSOBY
(
  ID INTEGER NOT NULL,
  IMIE VARCHAR(128) NOT NULL,
  NAZWISKO VARCHAR(128) NOT NULL,
  IM_OJCA VARCHAR(128) NOT NULL,
  IM_MATKI VARCHAR(128) NOT NULL,
  CONSTRAINT PK_OSOBY_0 PRIMARY KEY (ID),
  CONSTRAINT UNQ_OSOBY_0 UNIQUE (IMIE,NAZWISKO,IM_OJCA,IM_MATKI)
);
CREATE TABLE PLIKI
(
  ID INTEGER NOT NULL,
  KATALOG VARCHAR(512) NOT NULL,
  NAZWA VARCHAR(128) NOT NULL,
  ID_RODZAJU INTEGER NOT NULL,
  OPIS VARCHAR(1024) DEFAULT null,
  CONSTRAINT PK_PLIKI_0 PRIMARY KEY (ID),
  CONSTRAINT UNQ_PLIKI_0 UNIQUE (KATALOG,NAZWA)
);
CREATE TABLE RODZAJEDOK
(
  ID INTEGER NOT NULL,
  NAZWA VARCHAR(256) NOT NULL,
  CONSTRAINT PK_RODZAJEDOK_0 PRIMARY KEY (ID),
  CONSTRAINT UNQ_RODZAJEDOK_0 UNIQUE (NAZWA)
);

/******************** TRIGGERS ********************/

SET TERM ^ ;
CREATE TRIGGER DOKUMENTY_BI FOR DOKUMENTY ACTIVE
BEFORE INSERT POSITION 0
AS
DECLARE VARIABLE tmp DECIMAL(18,0);
BEGIN
  IF (NEW.ID IS NULL OR NEW.ID = 0) THEN
    NEW.ID = GEN_ID(GEN_DOKUMENTY_ID, 1);
END^
SET TERM ; ^
SET TERM ^ ;
CREATE TRIGGER DOK_DZIALKI_BI FOR DOK_DZIALKI ACTIVE
BEFORE INSERT POSITION 0
AS
DECLARE VARIABLE tmp DECIMAL(18,0);
BEGIN
  IF (NEW.ID IS NULL OR NEW.ID = 0) THEN
    NEW.ID = GEN_ID(GEN_DOK_DZIALKI_ID, 1);
END^
SET TERM ; ^
SET TERM ^ ;
CREATE TRIGGER DOK_OSOBY_BI FOR DOK_OSOBY ACTIVE
BEFORE INSERT POSITION 0
AS
DECLARE VARIABLE tmp DECIMAL(18,0);
BEGIN
  IF (NEW.ID IS NULL OR NEW.ID = 0) THEN
    NEW.ID = GEN_ID(GEN_DOK_OSOBY_ID, 1);
END^
SET TERM ; ^
SET TERM ^ ;
CREATE TRIGGER DOK_PLIKI_BI FOR DOK_PLIKI ACTIVE
BEFORE INSERT POSITION 0
AS
DECLARE VARIABLE tmp DECIMAL(18,0);
BEGIN
  IF (NEW.ID IS NULL OR NEW.ID = 0) THEN
    NEW.ID = GEN_ID(GEN_DOK_PLIKI_ID, 1);
END^
SET TERM ; ^
SET TERM ^ ;
CREATE TRIGGER DZIALKI_BI FOR DZIALKI ACTIVE
BEFORE INSERT POSITION 0
AS
DECLARE VARIABLE tmp DECIMAL(18,0);
BEGIN
  IF (NEW.ID IS NULL OR NEW.ID = 0) THEN
    NEW.ID = GEN_ID(GEN_DZIALKI_ID, 1);
END^
SET TERM ; ^
SET TERM ^ ;
CREATE TRIGGER GMINY_BI FOR GMINY ACTIVE
BEFORE INSERT POSITION 0
AS
DECLARE VARIABLE tmp DECIMAL(18,0);
BEGIN
  IF (NEW.ID IS NULL OR NEW.ID = 0) THEN
    NEW.ID = GEN_ID(GEN_GMINY_ID, 1);
END^
SET TERM ; ^
SET TERM ^ ;
CREATE TRIGGER OBREBY_BI FOR OBREBY ACTIVE
BEFORE INSERT POSITION 0
AS
DECLARE VARIABLE tmp DECIMAL(18,0);
BEGIN
  IF (NEW.ID IS NULL OR NEW.ID = 0) THEN
    NEW.ID = GEN_ID(GEN_OBREBY_ID, 1);
END^
SET TERM ; ^
SET TERM ^ ;
CREATE TRIGGER OSOBY_BI FOR OSOBY ACTIVE
BEFORE INSERT POSITION 0
AS
DECLARE VARIABLE tmp DECIMAL(18,0);
BEGIN
  IF (NEW.ID IS NULL OR NEW.ID = 0) THEN
    NEW.ID = GEN_ID(GEN_OSOBY_ID, 1);
END^
SET TERM ; ^
SET TERM ^ ;
CREATE TRIGGER PLIKI_BI FOR PLIKI ACTIVE
BEFORE INSERT POSITION 0
AS
DECLARE VARIABLE tmp DECIMAL(18,0);
BEGIN
  IF (NEW.ID IS NULL OR NEW.ID = 0) THEN
    NEW.ID = GEN_ID(GEN_PLIKI_ID, 1);
END^
SET TERM ; ^
SET TERM ^ ;
CREATE TRIGGER RODZAJEDOK_BI FOR RODZAJEDOK ACTIVE
BEFORE INSERT POSITION 0
AS
DECLARE VARIABLE tmp DECIMAL(18,0);
BEGIN
  IF (NEW.ID IS NULL OR NEW.ID = 0) THEN
    NEW.ID = GEN_ID(GEN_RODZAJEDOK_ID, 1);
END^
SET TERM ; ^


/******************** RELATIONS ********************/

ALTER TABLE DOKUMENTY ADD CONSTRAINT CHK_DOKUMENTY_0
  CHECK (TRIM(nazwa) <> '');

ALTER TABLE DOK_DZIALKI ADD CONSTRAINT FK_DOK_DZIALKI_0
  FOREIGN KEY (ID_DOK) REFERENCES DOKUMENTY (ID) ON UPDATE CASCADE ON DELETE CASCADE;
ALTER TABLE DOK_DZIALKI ADD CONSTRAINT FK_DOK_DZIALKI_1
  FOREIGN KEY (ID_DZI) REFERENCES DZIALKI (ID) ON UPDATE CASCADE;

ALTER TABLE DOK_OSOBY ADD CONSTRAINT FK_DOK_OSOBY_0
  FOREIGN KEY (ID_DOK) REFERENCES DOKUMENTY (ID) ON UPDATE CASCADE ON DELETE CASCADE;
ALTER TABLE DOK_OSOBY ADD CONSTRAINT FK_DOK_OSOBY_1
  FOREIGN KEY (ID_OSO) REFERENCES OSOBY (ID) ON UPDATE CASCADE;

ALTER TABLE DOK_PLIKI ADD CONSTRAINT FK_DOK_PLIKI_0
  FOREIGN KEY (ID_DOK) REFERENCES DOKUMENTY (ID) ON UPDATE CASCADE ON DELETE CASCADE;
ALTER TABLE DOK_PLIKI ADD CONSTRAINT FK_DOK_PLIKI_1
  FOREIGN KEY (ID_PLI) REFERENCES PLIKI (ID) ON UPDATE CASCADE;

ALTER TABLE DZIALKI ADD CONSTRAINT FK_DZIALKI_0
  FOREIGN KEY (ID_OBREBU) REFERENCES OBREBY (ID) ON UPDATE CASCADE;
ALTER TABLE DZIALKI ADD CONSTRAINT CHK_DZIALKI_0
  CHECK (TRIM(numer) <> '');

ALTER TABLE GMINY ADD CONSTRAINT CHK_GMINY_0
  CHECK (TRIM(nazwa) <> '');

ALTER TABLE OBREBY ADD CONSTRAINT FK_OBREBY_0
  FOREIGN KEY (ID_GMINY) REFERENCES GMINY (ID) ON UPDATE CASCADE;
ALTER TABLE OBREBY ADD CONSTRAINT CHK_OBREBY_0
  CHECK (TRIM(nazwa) <> '');

ALTER TABLE OSOBY ADD CONSTRAINT CHK_OSOBY_0
  CHECK (TRIM(imie) <> '' AND TRIM(nazwisko) <> '' AND TRIM(im_ojca) <> '' AND TRIM(im_matki) <> '');

ALTER TABLE PLIKI ADD CONSTRAINT FK_PLIKI_0
  FOREIGN KEY (ID_RODZAJU) REFERENCES RODZAJEDOK (ID) ON UPDATE CASCADE;
ALTER TABLE PLIKI ADD CONSTRAINT CHK_PLIKI_0
  CHECK (TRIM(nazwa) <> '' AND TRIM(katalog) <> '');

ALTER TABLE RODZAJEDOK ADD CONSTRAINT CHK_RODZAJEDOK_0
  CHECK (TRIM(nazwa) <> '');

/******************** COMMENTS ********************/

COMMENT ON COLUMN DOKUMENTY.ID is 'Identyfikator';
COMMENT ON COLUMN DOKUMENTY.NAZWA is 'Nazwa AWZ';
COMMENT ON COLUMN DOKUMENTY.OPIS is 'Uwagi';

COMMENT ON COLUMN DOK_DZIALKI.ID is 'Identyfikator';
COMMENT ON COLUMN DOK_DZIALKI.ID_DOK is 'ID dokuemntu AWZ';
COMMENT ON COLUMN DOK_DZIALKI.ID_DZI is 'ID działki';

COMMENT ON COLUMN DOK_OSOBY.ID is 'Identyfikator';
COMMENT ON COLUMN DOK_OSOBY.ID_DOK is 'ID dokumentu AWZ';
COMMENT ON COLUMN DOK_OSOBY.ID_OSO is 'ID właściciela';

COMMENT ON COLUMN DOK_PLIKI.ID is 'Identyfikator';
COMMENT ON COLUMN DOK_PLIKI.ID_DOK is 'ID dokumentu AWZ';
COMMENT ON COLUMN DOK_PLIKI.ID_PLI is 'ID skanu dokumentu';

COMMENT ON COLUMN DZIALKI.ID is 'Identyfikator';
COMMENT ON COLUMN DZIALKI.NUMER is 'Numer działki';
COMMENT ON COLUMN DZIALKI.ARKUSZ is 'Arkusz mapy';
COMMENT ON COLUMN DZIALKI.ID_OBREBU is 'ID obrębu';

COMMENT ON COLUMN GMINY.ID is 'Identyfikator';
COMMENT ON COLUMN GMINY.NAZWA is 'Nazwa gminy';

COMMENT ON COLUMN OBREBY.ID is 'Identyfikator';
COMMENT ON COLUMN OBREBY.NAZWA is 'Nazwa obrębu';
COMMENT ON COLUMN OBREBY.ID_GMINY is 'ID gminy';

COMMENT ON COLUMN OSOBY.ID is 'Identyfikator';
COMMENT ON COLUMN OSOBY.IMIE is 'Imię';
COMMENT ON COLUMN OSOBY.NAZWISKO is 'Nazwisko';
COMMENT ON COLUMN OSOBY.IM_OJCA is 'Imię ojca';
COMMENT ON COLUMN OSOBY.IM_MATKI is 'Imię matki';

COMMENT ON COLUMN PLIKI.ID is 'Identyfikator';
COMMENT ON COLUMN PLIKI.KATALOG is 'Folder pliku';
COMMENT ON COLUMN PLIKI.NAZWA is 'Nazwa pliku';
COMMENT ON COLUMN PLIKI.ID_RODZAJU is 'ID rodzaju dokumentu';
COMMENT ON COLUMN PLIKI.OPIS is 'Uwagi';

COMMENT ON COLUMN RODZAJEDOK.ID is 'Identyfikator';
COMMENT ON COLUMN RODZAJEDOK.NAZWA is 'Rodzaj dokumentu';

/******************** PRIVILEGES ********************/

CREATE ROLE ODCZYT;
CREATE ROLE EDYCJA;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON DOKUMENTY TO  SYSDBA WITH GRANT OPTION;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON DOK_DZIALKI TO  SYSDBA WITH GRANT OPTION;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON DOK_OSOBY TO  SYSDBA WITH GRANT OPTION;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON DOK_PLIKI TO  SYSDBA WITH GRANT OPTION;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON DZIALKI TO  SYSDBA WITH GRANT OPTION;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON GMINY TO  SYSDBA WITH GRANT OPTION;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON OBREBY TO  SYSDBA WITH GRANT OPTION;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON OSOBY TO  SYSDBA WITH GRANT OPTION;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON PLIKI TO  SYSDBA WITH GRANT OPTION;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON RODZAJEDOK TO  SYSDBA WITH GRANT OPTION;

GRANT SELECT, REFERENCES
 ON DOKUMENTY TO ROLE ODCZYT;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON DOK_DZIALKI TO ROLE ODCZYT;

GRANT SELECT, REFERENCES
 ON DOK_OSOBY TO ROLE ODCZYT;

GRANT SELECT, REFERENCES
 ON DOK_PLIKI TO ROLE ODCZYT;

GRANT SELECT, REFERENCES
 ON DZIALKI TO ROLE ODCZYT;

GRANT SELECT, REFERENCES
 ON GMINY TO ROLE ODCZYT;

GRANT SELECT, REFERENCES
 ON OBREBY TO ROLE ODCZYT;

GRANT SELECT, REFERENCES
 ON OSOBY TO ROLE ODCZYT;

GRANT SELECT, REFERENCES
 ON PLIKI TO ROLE ODCZYT;

GRANT SELECT, REFERENCES
 ON RODZAJEDOK TO ROLE ODCZYT;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON DOKUMENTY TO ROLE EDYCJA;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON DOK_DZIALKI TO ROLE EDYCJA;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON DOK_OSOBY TO ROLE EDYCJA;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON DOK_PLIKI TO ROLE EDYCJA;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON DZIALKI TO ROLE EDYCJA;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON GMINY TO ROLE EDYCJA;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON OBREBY TO ROLE EDYCJA;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON OSOBY TO ROLE EDYCJA;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON PLIKI TO ROLE EDYCJA;

GRANT DELETE, INSERT, REFERENCES, SELECT, UPDATE
 ON RODZAJEDOK TO ROLE EDYCJA;