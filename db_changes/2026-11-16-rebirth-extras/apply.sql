-- Adds the rebirth EXP and gold bonuses and the mailed reward per rebirth: columns bExpBonus and bGoldBonus
-- on TCHARREBIRTHTABLE, column dwPostID on TCHARREBIRTHLOGTABLE, and table TPORTALREBIRTHREWARD.
-- Additive: existing rebirth rows are kept (new columns default to 0). Safe to run twice.
-- Apply AFTER 2026-11-15-rebirth, BEFORE the portal's feat/rebirth-extras is merged and BEFORE a TMapSvr
-- built from feature/rebirth-extras starts. TPORTALREBIRTHREWARD is kept through the season reset.
--   sqlcmd -S "localhost\FOURSTORY" -E -C -b -d TGAME_GSP -i apply.sql
-- Runs as one transaction: any error stops the script and leaves the database unchanged.
:on error exit
SET NOCOUNT ON;
IF DB_NAME() NOT IN ('TGAME_GSP', 'TGAME_GSP_TEST') THROW 50000, 'Run this in TGAME_GSP or TGAME_GSP_TEST, using sqlcmd -d.', 1;
IF OBJECT_ID('dbo.TCHARREBIRTHTABLE', 'U') IS NULL
	THROW 50000, 'TCHARREBIRTHTABLE does not exist in this database. Apply 2026-11-15-rebirth first.', 1;
IF OBJECT_ID('dbo.TCHARREBIRTHLOGTABLE', 'U') IS NULL
	THROW 50000, 'TCHARREBIRTHLOGTABLE does not exist in this database. Apply 2026-11-15-rebirth first.', 1;
BEGIN TRANSACTION;
GO

IF COL_LENGTH('dbo.TCHARREBIRTHTABLE', 'bExpBonus') IS NULL
	ALTER TABLE dbo.TCHARREBIRTHTABLE ADD bExpBonus TINYINT NOT NULL CONSTRAINT DF_TCHARREBIRTHTABLE_bExpBonus DEFAULT 0 CONSTRAINT CK_TCHARREBIRTHTABLE_bExpBonus CHECK (bExpBonus <= 250);
GO
IF COL_LENGTH('dbo.TCHARREBIRTHTABLE', 'bGoldBonus') IS NULL
	ALTER TABLE dbo.TCHARREBIRTHTABLE ADD bGoldBonus TINYINT NOT NULL CONSTRAINT DF_TCHARREBIRTHTABLE_bGoldBonus DEFAULT 0 CONSTRAINT CK_TCHARREBIRTHTABLE_bGoldBonus CHECK (bGoldBonus <= 250);
GO
IF COL_LENGTH('dbo.TCHARREBIRTHLOGTABLE', 'dwPostID') IS NULL
	ALTER TABLE dbo.TCHARREBIRTHLOGTABLE ADD dwPostID INT NOT NULL CONSTRAINT DF_TCHARREBIRTHLOGTABLE_dwPostID DEFAULT 0;	-- the reward letter, 0 for none
GO

IF OBJECT_ID('dbo.TPORTALREBIRTHREWARD', 'U') IS NULL
	CREATE TABLE dbo.TPORTALREBIRTHREWARD (
		bRebirth		TINYINT		NOT NULL CONSTRAINT PK_TPORTALREBIRTHREWARD PRIMARY KEY CONSTRAINT CK_TPORTALREBIRTHREWARD_bRebirth CHECK (bRebirth BETWEEN 1 AND 20),
		dwGold			INT			NOT NULL,
		dwSilver		INT			NOT NULL,
		dwCooper		INT			NOT NULL,
		wItemID1		SMALLINT	NOT NULL,	-- 0 for an empty slot
		bCount1			TINYINT		NOT NULL,
		wItemID2		SMALLINT	NOT NULL,
		bCount2			TINYINT		NOT NULL,
		wItemID3		SMALLINT	NOT NULL,
		bCount3			TINYINT		NOT NULL,
		szUpdatedBy		VARCHAR(50)	NOT NULL,
		dUpdated		DATETIME	NOT NULL CONSTRAINT DF_TPORTALREBIRTHREWARD_dUpdated DEFAULT GETDATE()
	);
GO

COMMIT;
PRINT 'Rebirth EXP and gold bonus columns and TPORTALREBIRTHREWARD ready in ' + DB_NAME();
GO
