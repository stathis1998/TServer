-- Undo: removes the Wheel of Fortune's procedure and its two tables.
-- Every recorded spin and every prize a GM set are lost, and so is the switch. Letters already sent stay in the mailboxes.
-- TPORTALSETTING itself stays: achievements use it too.
-- Revert the portal first: with the tables gone its wheel page shows a closed, empty wheel and nothing can be saved.
USE TGAME_GSP;
GO
DROP PROCEDURE IF EXISTS dbo.TPortalWheelSpin;
GO
DROP TABLE IF EXISTS dbo.TPORTALWHEELSPINTABLE;
DROP TABLE IF EXISTS dbo.TPORTALWHEELPRIZE;
IF OBJECT_ID('dbo.TPORTALSETTING', 'U') IS NOT NULL DELETE FROM dbo.TPORTALSETTING WHERE szKey = 'wheel_live';
GO
