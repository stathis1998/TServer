-- Undo: removes the cascades, the meter, the Mini and Major pots and the daily limit's index.
--   TPORTALSLOTJACKPOT  rows 2 (Mini) and 3 (Major) are deleted and the check goes back to bID = 1. The gold in them is lost
--                       with the rows: let them drop first, or accept that it is gone. Row 1 (the Giga) is not touched.
--   TPORTALSLOTMETER    dropped. Every account's meter points are lost.
--   TPORTALSLOTSPIN     the columns szMeterFree, llAddMini, llAddMajor, llAddGiga, llWonMini, llWonMajor, llWonGiga and bMeterBefore
--                       are dropped with their default constraints, and so is the index IX_TPORTALSLOTSPIN_USER_TIME.
--                       szFree is narrowed back to VARCHAR(400) only if every row's text fits; otherwise it stays at 800
--                       and a message says so (only rounds with more than about 40 free spins do not fit).
--                       The table and its rows stay, as do the other settings rows.
-- The API from before this change must be running again before this undo: the new one writes these columns on every spin and
-- reads the meter table, so with them gone every spin would fail.
-- Cascade rounds already played (bRules 3) keep their row; their cascades, meter free spins and each pot's share are lost with
-- the columns. Wallets stay as they are. llAdded and llJackpot stay, but they hold all three pots' money: after this undo
-- the old GM page's jackpot Added and Paid also count Mini and Major money the single pot never held.
-- The setting rows this change introduced (slots_jackpot_mini_*, slots_jackpot_major_* and slots_daily_rounds) are deleted;
-- slots_jackpot_live and slots_jackpot_cap (the Giga) and the other slots keys are not touched.
USE TGAME_GSP;
GO
DROP TABLE IF EXISTS dbo.TPORTALSLOTMETER;
GO
-- The Mini and Major pots go, and the check goes back to allowing row 1 only.
IF OBJECT_ID('dbo.TPORTALSLOTJACKPOT', 'U') IS NOT NULL
BEGIN
	DELETE FROM dbo.TPORTALSLOTJACKPOT WHERE bID IN (2, 3);
	IF EXISTS (SELECT 1 FROM sys.check_constraints WHERE parent_object_id = OBJECT_ID('dbo.TPORTALSLOTJACKPOT') AND name = 'CK_TPORTALSLOTJACKPOT_ID' AND definition <> '([bID]=(1))')
		ALTER TABLE dbo.TPORTALSLOTJACKPOT DROP CONSTRAINT CK_TPORTALSLOTJACKPOT_ID;
	IF NOT EXISTS (SELECT 1 FROM sys.check_constraints WHERE parent_object_id = OBJECT_ID('dbo.TPORTALSLOTJACKPOT') AND name = 'CK_TPORTALSLOTJACKPOT_ID')
		ALTER TABLE dbo.TPORTALSLOTJACKPOT ADD CONSTRAINT CK_TPORTALSLOTJACKPOT_ID CHECK (bID = 1);
END
GO
-- The index goes, then the default constraints before the columns, or the column drop is refused.
IF OBJECT_ID('dbo.TPORTALSLOTSPIN', 'U') IS NOT NULL
BEGIN
	DROP INDEX IF EXISTS IX_TPORTALSLOTSPIN_USER_TIME ON dbo.TPORTALSLOTSPIN;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP CONSTRAINT IF EXISTS DF_TPORTALSLOTSPIN_szMeterFree;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP CONSTRAINT IF EXISTS DF_TPORTALSLOTSPIN_llAddMini;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP CONSTRAINT IF EXISTS DF_TPORTALSLOTSPIN_llAddMajor;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP CONSTRAINT IF EXISTS DF_TPORTALSLOTSPIN_llAddGiga;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP CONSTRAINT IF EXISTS DF_TPORTALSLOTSPIN_llWonMini;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP CONSTRAINT IF EXISTS DF_TPORTALSLOTSPIN_llWonMajor;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP CONSTRAINT IF EXISTS DF_TPORTALSLOTSPIN_llWonGiga;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP CONSTRAINT IF EXISTS DF_TPORTALSLOTSPIN_bMeterBefore;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP COLUMN IF EXISTS szMeterFree;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP COLUMN IF EXISTS llAddMini;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP COLUMN IF EXISTS llAddMajor;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP COLUMN IF EXISTS llAddGiga;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP COLUMN IF EXISTS llWonMini;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP COLUMN IF EXISTS llWonMajor;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP COLUMN IF EXISTS llWonGiga;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP COLUMN IF EXISTS bMeterBefore;
END
GO
-- szFree back to 400, only when no row's text is longer. The change is run as a string so the batch compiles whichever way the check goes.
IF COL_LENGTH('dbo.TPORTALSLOTSPIN', 'szFree') > 400
BEGIN
	IF EXISTS (SELECT 1 FROM dbo.TPORTALSLOTSPIN WHERE DATALENGTH(szFree) > 400)
		PRINT 'szFree left at VARCHAR(800): a row holds more than 400 characters of free spins and would be cut.';
	ELSE
		EXEC('ALTER TABLE dbo.TPORTALSLOTSPIN ALTER COLUMN szFree VARCHAR(400) NOT NULL');
END
GO
-- The setting rows this change introduced. The Giga's keys (slots_jackpot_live, slots_jackpot_cap) are older and stay.
IF OBJECT_ID('dbo.TPORTALSETTING', 'U') IS NOT NULL
	DELETE FROM dbo.TPORTALSETTING WHERE szKey IN ('slots_daily_rounds', 'slots_jackpot_mini_live', 'slots_jackpot_mini_cap', 'slots_jackpot_major_live', 'slots_jackpot_major_cap');
GO
