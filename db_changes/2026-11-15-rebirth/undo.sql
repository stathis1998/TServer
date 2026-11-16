-- Removes the rebirth tables and DELETES every rebirth recorded in them, and the four rebirth_ settings
-- in TPORTALSETTING (price, percent, limit and the on/off switch) when that table exists.
-- Characters keep their level and gear, but lose the rebirth number and the stat bonus.
-- Run a TMapSvr build from before the rebirth bonus first. A newer build keeps working without the
-- table, but logs a failed read at every character load and gives every character bonus 0.
--   sqlcmd -S "localhost\FOURSTORY" -E -C -b -d TGAME_GSP -i undo.sql
SET NOCOUNT ON;
IF OBJECT_ID('dbo.TCHARREBIRTHTABLE', 'U') IS NOT NULL DROP TABLE dbo.TCHARREBIRTHTABLE;
IF OBJECT_ID('dbo.TCHARREBIRTHLOGTABLE', 'U') IS NOT NULL DROP TABLE dbo.TCHARREBIRTHLOGTABLE;
IF OBJECT_ID('dbo.TPORTALSETTING', 'U') IS NOT NULL
	EXEC (N'DELETE FROM dbo.TPORTALSETTING WHERE szKey LIKE ''rebirth[_]%''');
PRINT 'Rebirth tables and settings removed from ' + DB_NAME();
GO
