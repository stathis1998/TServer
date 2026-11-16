-- Apply: the slot machines' shared jackpot.
--   TPORTALSLOTJACKPOT   new one-row table: the pot, and the cap it is running under, both in Bronze
--   TPORTALSLOTSPIN      two new columns, llAdded and llJackpot (what a spin added to the pot, what it was paid from it), 0 by default
-- Needs the slots script (db_changes\2026-11-14-portal-slots) applied first: this adds columns to its table TPORTALSLOTSPIN.
-- Source of truth: 4story-portal apps/api/sql/, one file per object (TPORTALSLOTJACKPOT.sql and TPORTALSLOTSPIN.sql).
-- Safe to run twice. No restart needed.
-- The SQL has to be in the database before the new portal API spins against it. Until then every spin is refused
-- with "Slots are not available right now" and the slots page shows no jackpot.
-- No setting rows are added. With none, the jackpot is on with a 50 Gold cap. The keys are slots_jackpot_live and slots_jackpot_cap.
USE TGAME_GSP;
GO
-- The slot machines' shared jackpot: one row, grown by lost rounds and emptied when it drops. It is changed in
-- the same transaction as the spin that moves it, so the pot and the wallets never disagree.
--   Money is in Bronze (1 Silver = 1000 Bronze).
--   llCap is the cap, in Bronze, that the pot runs under: set when a lost round first adds to an empty pot, and
--   NULL until then. A GM changing the cap later does not change a pot already growing.
IF OBJECT_ID('dbo.TPORTALSLOTJACKPOT', 'U') IS NULL
BEGIN
	CREATE TABLE dbo.TPORTALSLOTJACKPOT (
		bID				TINYINT			NOT NULL CONSTRAINT PK_TPORTALSLOTJACKPOT PRIMARY KEY CONSTRAINT CK_TPORTALSLOTJACKPOT_ID CHECK (bID = 1),
		llPot			BIGINT			NOT NULL CONSTRAINT DF_TPORTALSLOTJACKPOT_llPot DEFAULT 0,	-- Bronze in the pot
		llCap			BIGINT			NULL		-- Bronze, the cap this pot runs under
	);
END
IF NOT EXISTS (SELECT 1 FROM dbo.TPORTALSLOTJACKPOT WHERE bID = 1)
	INSERT INTO dbo.TPORTALSLOTJACKPOT (bID) VALUES (1);
GO
-- llAdded and llJackpot are the jackpot's part of a spin: what a lost round put in the pot, what a win took out.
IF COL_LENGTH('dbo.TPORTALSLOTSPIN', 'llAdded') IS NULL
	ALTER TABLE dbo.TPORTALSLOTSPIN ADD llAdded BIGINT NOT NULL CONSTRAINT DF_TPORTALSLOTSPIN_llAdded DEFAULT 0;
IF COL_LENGTH('dbo.TPORTALSLOTSPIN', 'llJackpot') IS NULL
	ALTER TABLE dbo.TPORTALSLOTSPIN ADD llJackpot BIGINT NOT NULL CONSTRAINT DF_TPORTALSLOTSPIN_llJackpot DEFAULT 0;
GO
