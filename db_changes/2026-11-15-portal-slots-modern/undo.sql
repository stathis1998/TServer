-- Undo: removes the five-reel machines' columns.
-- Drops the columns bStop4, bStop5, bRules and szFree from TPORTALSLOTSPIN, with their default constraints.
-- TPORTALSLOTSPIN and its rows stay, as do the settings.
-- The API from before this change must be running again before this undo: the new one writes these four columns on every spin,
-- so with the columns gone every spin would fail.
-- Five-reel rounds already played lose their free spins and their last two stops with the columns, so the old API reads them
-- as three-reel rounds. Wallets stay as they are.
USE TGAME_GSP;
GO
-- The default constraints go before the columns, or the column drop is refused.
IF OBJECT_ID('dbo.TPORTALSLOTSPIN', 'U') IS NOT NULL
BEGIN
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP CONSTRAINT IF EXISTS DF_TPORTALSLOTSPIN_bStop4;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP CONSTRAINT IF EXISTS DF_TPORTALSLOTSPIN_bStop5;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP CONSTRAINT IF EXISTS DF_TPORTALSLOTSPIN_bRules;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP CONSTRAINT IF EXISTS DF_TPORTALSLOTSPIN_szFree;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP COLUMN IF EXISTS bStop4;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP COLUMN IF EXISTS bStop5;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP COLUMN IF EXISTS bRules;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP COLUMN IF EXISTS szFree;
END
GO
