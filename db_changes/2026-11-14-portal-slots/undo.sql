-- Undo: removes the slot machines' spin table and every slots setting.
-- Every recorded spin is lost (the history and the big wins list). Wallets stay as they are: money already won or lost is not given back.
-- TPORTALSETTING itself stays: achievements, the wheel and roulette use it too. Roulette's and the wheel's keys are left alone.
-- Revert the portal first: with the table gone the slots page cannot record a spin and the GM page cannot show totals.
USE TGAME_GSP;
GO
DROP TABLE IF EXISTS dbo.TPORTALSLOTSPIN;
IF OBJECT_ID('dbo.TPORTALSETTING', 'U') IS NOT NULL
	DELETE FROM dbo.TPORTALSETTING WHERE szKey LIKE 'slots[_]%';
GO
