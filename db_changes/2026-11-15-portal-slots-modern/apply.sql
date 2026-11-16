-- Apply: the slot machines become five-reel machines with 20 win lines, a wild, a scatter and free spins.
--   TPORTALSLOTSPIN   four new columns: bStop4 and bStop5 (the last two reels' stops), bRules (1: three reels, the old rounds; 2: five reels)
--                     and szFree (the free spins' stops, as text, empty when there were none)
-- Needs the slots script (db_changes\2026-11-14-portal-slots) and the jackpot script (db_changes\2026-11-15-portal-slots-jackpot)
-- applied first: this adds columns to their table TPORTALSLOTSPIN.
-- Source of truth: 4story-portal apps/api/sql/TPORTALSLOTSPIN.sql (the guarded ALTER TABLE blocks at its end).
-- Safe to run twice. No restart needed. Rows already there keep rules 1, stops 0 for the two new reels, and no free spins.
-- The SQL has to be in the database before the new portal API spins against it. Until then every spin is refused
-- with "Slots are not available right now".
-- No setting rows are added. A GM-saved stake that is not a multiple of 20 Silver falls back to the machine's starting stakes.
USE TGAME_GSP;
GO
-- bStop4 and bStop5 are the fourth and fifth reel's stops. An old three-reel row reads 0 in both and rules 1.
IF COL_LENGTH('dbo.TPORTALSLOTSPIN', 'bStop4') IS NULL
	ALTER TABLE dbo.TPORTALSLOTSPIN ADD bStop4 TINYINT NOT NULL CONSTRAINT DF_TPORTALSLOTSPIN_bStop4 DEFAULT 0;
IF COL_LENGTH('dbo.TPORTALSLOTSPIN', 'bStop5') IS NULL
	ALTER TABLE dbo.TPORTALSLOTSPIN ADD bStop5 TINYINT NOT NULL CONSTRAINT DF_TPORTALSLOTSPIN_bStop5 DEFAULT 0;
-- bRules says which machine rules played the round: 1 is the old three reels, 2 the five reels.
IF COL_LENGTH('dbo.TPORTALSLOTSPIN', 'bRules') IS NULL
	ALTER TABLE dbo.TPORTALSLOTSPIN ADD bRules TINYINT NOT NULL CONSTRAINT DF_TPORTALSLOTSPIN_bRules DEFAULT 1;
-- szFree holds every free spin's stops in the order played, empty for a round without any.
IF COL_LENGTH('dbo.TPORTALSLOTSPIN', 'szFree') IS NULL
	ALTER TABLE dbo.TPORTALSLOTSPIN ADD szFree VARCHAR(400) NOT NULL CONSTRAINT DF_TPORTALSLOTSPIN_szFree DEFAULT '';
GO
