-- Undo: removes the achievement procedure and its three tables.
-- Every recorded award, every reward a GM set and the switch are lost. Letters already sent stay in the mailboxes.
-- Revert the portal first: with the tables gone its achievement pages show levels only and nothing can be saved.
USE TGAME_GSP;
GO
DROP PROCEDURE IF EXISTS dbo.TPortalAchievementAward;
GO
DROP TABLE IF EXISTS dbo.TPORTALACHIEVEMENTTABLE;
DROP TABLE IF EXISTS dbo.TPORTALACHIEVEMENTREWARD;
DROP TABLE IF EXISTS dbo.TPORTALSETTING;
GO
