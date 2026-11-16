-- Adds portal rebirth: tables TCHARREBIRTHTABLE (a character's rebirth count and stat bonus percent) and
-- TCHARREBIRTHLOGTABLE (one row per rebirth, with the announcement text and the achievement baseline).
-- Purely additive: changes no existing table or procedure, and the running server ignores it.
-- Apply BEFORE the portal's feat/rebirth is merged and BEFORE a TMapSvr built from feature/rebirth-bonus starts.
--   sqlcmd -S "localhost\FOURSTORY" -E -C -b -d TGAME_GSP -i apply.sql
-- Runs as one transaction: any error stops the script and leaves the database unchanged.
:on error exit
SET NOCOUNT ON;
IF DB_NAME() NOT IN ('TGAME_GSP', 'TGAME_GSP_TEST') THROW 50000, 'Run this in TGAME_GSP or TGAME_GSP_TEST, using sqlcmd -d.', 1;
IF OBJECT_ID('dbo.TCHARREBIRTHTABLE') IS NOT NULL
	THROW 50000, 'TCHARREBIRTHTABLE already exists in this database. Run undo.sql first.', 1;
IF OBJECT_ID('dbo.TCHARREBIRTHLOGTABLE') IS NOT NULL
	THROW 50000, 'TCHARREBIRTHLOGTABLE already exists in this database. Run undo.sql first.', 1;
BEGIN TRANSACTION;
GO

CREATE TABLE dbo.TCHARREBIRTHTABLE (
	dwCharID		INT			NOT NULL CONSTRAINT PK_TCHARREBIRTHTABLE PRIMARY KEY,
	bRebirth		TINYINT		NOT NULL CONSTRAINT CK_TCHARREBIRTHTABLE_bRebirth CHECK (bRebirth >= 1),
	bStatBonus		TINYINT		NOT NULL CONSTRAINT CK_TCHARREBIRTHTABLE_bStatBonus CHECK (bStatBonus <= 100),	-- percent
	dUpdate			DATETIME	NOT NULL CONSTRAINT DF_TCHARREBIRTHTABLE_dUpdate DEFAULT GETDATE()
);
GO

CREATE TABLE dbo.TCHARREBIRTHLOGTABLE (
	dwID			INT IDENTITY(1,1)	NOT NULL CONSTRAINT PK_TCHARREBIRTHLOGTABLE PRIMARY KEY,
	dwUserID		INT				NOT NULL,	-- the account
	dwCharID		INT				NOT NULL,
	szCharName		VARCHAR(50)		NOT NULL,
	bRebirth		TINYINT			NOT NULL,	-- the character's rebirth number after this one
	bStatBonus		TINYINT			NOT NULL,	-- percent, after this one
	dwPriceGold		INT				NOT NULL,	-- Gold paid
	szBaseline		VARCHAR(2000)	NOT NULL,	-- JSON
	szText			VARCHAR(160)	NOT NULL,	-- the announcement, names filled in
	bAnnounced		TINYINT			NOT NULL CONSTRAINT DF_TCHARREBIRTHLOGTABLE_bAnnounced DEFAULT 0,
	dRebirth		DATETIME		NOT NULL CONSTRAINT DF_TCHARREBIRTHLOGTABLE_dRebirth DEFAULT GETDATE(),
	CONSTRAINT UQ_TCHARREBIRTHLOGTABLE_CHAR_REBIRTH UNIQUE (dwCharID, bRebirth)
);
GO

COMMIT;
PRINT 'TCHARREBIRTHTABLE and TCHARREBIRTHLOGTABLE created in ' + DB_NAME();
GO
