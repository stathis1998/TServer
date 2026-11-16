-- Undo: removes the slot machines' shared jackpot.
-- Drops TPORTALSLOTJACKPOT, the columns llAdded and llJackpot from TPORTALSLOTSPIN (with their default constraints),
-- and the two setting rows slots_jackpot_live and slots_jackpot_cap. TPORTALSLOTSPIN and its rows stay, as do the other slots keys.
-- A pot that has not been won is lost with the table, so let it drop first, or accept that the gold in it is gone.
-- The API from before this change must be running again before this undo: the new one writes llAdded and llJackpot on every spin,
-- so with the columns gone every spin would fail.
-- The recorded amounts in llAdded and llJackpot are lost with their columns. Wallets stay as they are.
USE TGAME_GSP;
GO
DROP TABLE IF EXISTS dbo.TPORTALSLOTJACKPOT;
GO
-- The default constraints go before the columns, or the column drop is refused.
IF OBJECT_ID('dbo.TPORTALSLOTSPIN', 'U') IS NOT NULL
BEGIN
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP CONSTRAINT IF EXISTS DF_TPORTALSLOTSPIN_llAdded;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP CONSTRAINT IF EXISTS DF_TPORTALSLOTSPIN_llJackpot;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP COLUMN IF EXISTS llAdded;
	ALTER TABLE dbo.TPORTALSLOTSPIN DROP COLUMN IF EXISTS llJackpot;
END
GO
IF OBJECT_ID('dbo.TPORTALSETTING', 'U') IS NOT NULL
	DELETE FROM dbo.TPORTALSETTING WHERE szKey IN ('slots_jackpot_live', 'slots_jackpot_cap');
GO
