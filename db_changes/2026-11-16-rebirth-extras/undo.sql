-- Removes TPORTALREBIRTHREWARD (and every reward in it), the columns bExpBonus and bGoldBonus on
-- TCHARREBIRTHTABLE and dwPostID on TCHARREBIRTHLOGTABLE with their constraints, and the settings
-- rebirth_exp_percent and rebirth_gold_percent in TPORTALSETTING when that table exists.
-- Rebirth counts, stat bonuses and the rebirth log rows stay. Run a TMapSvr build from before the EXP and
-- gold bonuses first, as the newer one reads the two columns.
--   sqlcmd -S "localhost\FOURSTORY" -E -C -b -d TGAME_GSP -i undo.sql
:on error exit
SET NOCOUNT ON;
IF DB_NAME() NOT IN ('TGAME_GSP', 'TGAME_GSP_TEST') THROW 50000, 'Run this in TGAME_GSP or TGAME_GSP_TEST, using sqlcmd -d.', 1;
IF OBJECT_ID('dbo.TPORTALREBIRTHREWARD', 'U') IS NOT NULL DROP TABLE dbo.TPORTALREBIRTHREWARD;
GO
IF COL_LENGTH('dbo.TCHARREBIRTHTABLE', 'bExpBonus') IS NOT NULL
BEGIN
	ALTER TABLE dbo.TCHARREBIRTHTABLE DROP CONSTRAINT CK_TCHARREBIRTHTABLE_bExpBonus;
	ALTER TABLE dbo.TCHARREBIRTHTABLE DROP CONSTRAINT DF_TCHARREBIRTHTABLE_bExpBonus;
	ALTER TABLE dbo.TCHARREBIRTHTABLE DROP COLUMN bExpBonus;
END
GO
IF COL_LENGTH('dbo.TCHARREBIRTHTABLE', 'bGoldBonus') IS NOT NULL
BEGIN
	ALTER TABLE dbo.TCHARREBIRTHTABLE DROP CONSTRAINT CK_TCHARREBIRTHTABLE_bGoldBonus;
	ALTER TABLE dbo.TCHARREBIRTHTABLE DROP CONSTRAINT DF_TCHARREBIRTHTABLE_bGoldBonus;
	ALTER TABLE dbo.TCHARREBIRTHTABLE DROP COLUMN bGoldBonus;
END
GO
IF COL_LENGTH('dbo.TCHARREBIRTHLOGTABLE', 'dwPostID') IS NOT NULL
BEGIN
	ALTER TABLE dbo.TCHARREBIRTHLOGTABLE DROP CONSTRAINT DF_TCHARREBIRTHLOGTABLE_dwPostID;
	ALTER TABLE dbo.TCHARREBIRTHLOGTABLE DROP COLUMN dwPostID;
END
GO
IF OBJECT_ID('dbo.TPORTALSETTING', 'U') IS NOT NULL
	EXEC (N'DELETE FROM dbo.TPORTALSETTING WHERE szKey IN (''rebirth_exp_percent'', ''rebirth_gold_percent'')');
PRINT 'Rebirth EXP and gold bonus columns, rewards and settings removed from ' + DB_NAME();
GO
