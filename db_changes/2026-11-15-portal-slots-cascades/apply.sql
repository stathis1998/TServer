-- Apply: the slot machines get cascades, a meter, three jackpots and a daily limit.
--   TPORTALSLOTSPIN     szFree widened from VARCHAR(400) to VARCHAR(800) (50 free spins), and eight new columns:
--                       szMeterFree (the meter's free spins' stops), llAddMini, llAddMajor, llAddGiga and llWonMini, llWonMajor,
--                       llWonGiga (each pot's share of llAdded and llJackpot) and bMeterBefore (the meter's points before the round);
--                       a new index IX_TPORTALSLOTSPIN_USER_TIME for the daily count of an account's rounds
--   TPORTALSLOTMETER    new table: each account's meter, one row per account, machine and stake
--   TPORTALSLOTJACKPOT  the check on bID now allows 1 to 3, and rows 2 (Mini) and 3 (Major) are added next to row 1 (Giga, the pot from before)
-- Needs the slots script (db_changes\2026-11-14-portal-slots), the jackpot script (db_changes\2026-11-15-portal-slots-jackpot)
-- and the five-reel script (db_changes\2026-11-15-portal-slots-modern) applied first, in that order: this changes their tables.
-- Source of truth: 4story-portal apps/api/sql/, one file per object (TPORTALSLOTSPIN.sql, TPORTALSLOTJACKPOT.sql and TPORTALSLOTMETER.sql).
-- Safe to run twice. No restart needed. Rows already there keep their rules number and their free spins, and read 0 for each pot's
-- share, no meter free spins and a meter of 0.
-- The SQL has to be in the database before the new portal API spins against it. Do not deploy the new portal against a database
-- before this script has run there: until then every spin is refused with "Slots are not available right now".
-- No setting rows are added. With none, the pots are on with caps of 1, 10 and 50 Gold and the daily limit is 1,500 rounds.
-- The keys are slots_jackpot_mini_live, slots_jackpot_mini_cap, slots_jackpot_major_live, slots_jackpot_major_cap,
-- slots_jackpot_live and slots_jackpot_cap (the Giga, as before) and slots_daily_rounds.
-- A GM-saved stake above 5 Gold falls back to the machine's starting stakes.
USE TGAME_GSP;
GO
-- The free spins' text must hold up to 50 spins. An older row keeps its text.
IF COL_LENGTH('dbo.TPORTALSLOTSPIN', 'szFree') < 800
	ALTER TABLE dbo.TPORTALSLOTSPIN ALTER COLUMN szFree VARCHAR(800) NOT NULL;
-- The meter's free spins, each pot's share of what the round added and won, and the meter before the round.
IF COL_LENGTH('dbo.TPORTALSLOTSPIN', 'szMeterFree') IS NULL
	ALTER TABLE dbo.TPORTALSLOTSPIN ADD szMeterFree VARCHAR(800) NOT NULL CONSTRAINT DF_TPORTALSLOTSPIN_szMeterFree DEFAULT '';
IF COL_LENGTH('dbo.TPORTALSLOTSPIN', 'llAddMini') IS NULL
	ALTER TABLE dbo.TPORTALSLOTSPIN ADD llAddMini BIGINT NOT NULL CONSTRAINT DF_TPORTALSLOTSPIN_llAddMini DEFAULT 0;
IF COL_LENGTH('dbo.TPORTALSLOTSPIN', 'llAddMajor') IS NULL
	ALTER TABLE dbo.TPORTALSLOTSPIN ADD llAddMajor BIGINT NOT NULL CONSTRAINT DF_TPORTALSLOTSPIN_llAddMajor DEFAULT 0;
IF COL_LENGTH('dbo.TPORTALSLOTSPIN', 'llAddGiga') IS NULL
	ALTER TABLE dbo.TPORTALSLOTSPIN ADD llAddGiga BIGINT NOT NULL CONSTRAINT DF_TPORTALSLOTSPIN_llAddGiga DEFAULT 0;
IF COL_LENGTH('dbo.TPORTALSLOTSPIN', 'llWonMini') IS NULL
	ALTER TABLE dbo.TPORTALSLOTSPIN ADD llWonMini BIGINT NOT NULL CONSTRAINT DF_TPORTALSLOTSPIN_llWonMini DEFAULT 0;
IF COL_LENGTH('dbo.TPORTALSLOTSPIN', 'llWonMajor') IS NULL
	ALTER TABLE dbo.TPORTALSLOTSPIN ADD llWonMajor BIGINT NOT NULL CONSTRAINT DF_TPORTALSLOTSPIN_llWonMajor DEFAULT 0;
IF COL_LENGTH('dbo.TPORTALSLOTSPIN', 'llWonGiga') IS NULL
	ALTER TABLE dbo.TPORTALSLOTSPIN ADD llWonGiga BIGINT NOT NULL CONSTRAINT DF_TPORTALSLOTSPIN_llWonGiga DEFAULT 0;
IF COL_LENGTH('dbo.TPORTALSLOTSPIN', 'bMeterBefore') IS NULL
	ALTER TABLE dbo.TPORTALSLOTSPIN ADD bMeterBefore TINYINT NOT NULL CONSTRAINT DF_TPORTALSLOTSPIN_bMeterBefore DEFAULT 0;
-- bRules now also says 3 for a cascade round. The column is not changed: 1 and 2 stay in the totals and out of every list.
-- The daily limit counts an account's rounds since midnight.
IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE object_id = OBJECT_ID('dbo.TPORTALSLOTSPIN') AND name = 'IX_TPORTALSLOTSPIN_USER_TIME')
	CREATE INDEX IX_TPORTALSLOTSPIN_USER_TIME ON dbo.TPORTALSLOTSPIN (dwUserID, dSpun);
GO
-- Each account's meter on a machine at a stake. A paid spin whose last window shows one or two scatters adds that many points;
-- at the machine's size the meter pays its free spins and keeps the points over the size.
--   dwStake is whole Silver: points earned at one stake are only cashed at that stake.
IF OBJECT_ID('dbo.TPORTALSLOTMETER', 'U') IS NULL
BEGIN
	CREATE TABLE dbo.TPORTALSLOTMETER (
		dwUserID		INT				NOT NULL,	-- the account
		szMachine		VARCHAR(20)		NOT NULL,	-- 'shelf', 'armoury', 'hoard'
		dwStake			INT				NOT NULL,	-- Silver
		bPoints			TINYINT			NOT NULL,
		dUpdated		DATETIME		NOT NULL CONSTRAINT DF_TPORTALSLOTMETER_dUpdated DEFAULT GETDATE(),
		CONSTRAINT PK_TPORTALSLOTMETER PRIMARY KEY (dwUserID, szMachine, dwStake)
	);
END
GO
-- The pots: a table made before the three pots allows only row 1, so its check is replaced by one that allows rows 1 to 3.
-- Row 1 is the Giga (the pot from before, with everything it holds), 2 is the Mini, 3 the Major. An empty pot has no cap yet.
IF OBJECT_ID('dbo.TPORTALSLOTJACKPOT', 'U') IS NOT NULL
BEGIN
	IF EXISTS (SELECT 1 FROM sys.check_constraints WHERE parent_object_id = OBJECT_ID('dbo.TPORTALSLOTJACKPOT') AND name = 'CK_TPORTALSLOTJACKPOT_ID' AND definition = '([bID]=(1))')
		ALTER TABLE dbo.TPORTALSLOTJACKPOT DROP CONSTRAINT CK_TPORTALSLOTJACKPOT_ID;
	IF NOT EXISTS (SELECT 1 FROM sys.check_constraints WHERE parent_object_id = OBJECT_ID('dbo.TPORTALSLOTJACKPOT') AND name = 'CK_TPORTALSLOTJACKPOT_ID')
		ALTER TABLE dbo.TPORTALSLOTJACKPOT ADD CONSTRAINT CK_TPORTALSLOTJACKPOT_ID CHECK (bID BETWEEN 1 AND 3);
END
GO
IF OBJECT_ID('dbo.TPORTALSLOTJACKPOT', 'U') IS NOT NULL
BEGIN
	IF NOT EXISTS (SELECT 1 FROM dbo.TPORTALSLOTJACKPOT WHERE bID = 2)
		INSERT INTO dbo.TPORTALSLOTJACKPOT (bID) VALUES (2);
	IF NOT EXISTS (SELECT 1 FROM dbo.TPORTALSLOTJACKPOT WHERE bID = 3)
		INSERT INTO dbo.TPORTALSLOTJACKPOT (bID) VALUES (3);
END
GO
