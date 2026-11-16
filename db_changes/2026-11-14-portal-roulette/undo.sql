-- Undo: removes the roulette's spin table and its four settings.
-- Every recorded spin is lost (the history and the big wins list). Wallets stay as they are: money already won or lost is not given back.
-- TPORTALSETTING itself stays: achievements and the wheel use it too.
-- Revert the portal first: with the table gone the roulette page cannot record a spin and the GM page cannot show totals.
USE TGAME_GSP;
GO
DROP TABLE IF EXISTS dbo.TPORTALROULETTESPIN;
IF OBJECT_ID('dbo.TPORTALSETTING', 'U') IS NOT NULL
	DELETE FROM dbo.TPORTALSETTING WHERE szKey IN ('roulette_live', 'roulette_min_bet', 'roulette_max_number', 'roulette_max_outside');
GO
