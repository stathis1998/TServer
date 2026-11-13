-- Removes the general counters and DELETES every value recorded in them.
-- Run a TMapSvr build from before these counters first, or the running server will log failed saves.
--   sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -i undo.sql
SET NOCOUNT ON;
IF OBJECT_ID('dbo.TSaveCharCounter', 'P') IS NOT NULL DROP PROCEDURE dbo.TSaveCharCounter;
IF OBJECT_ID('dbo.TGetCharCounter', 'P') IS NOT NULL DROP PROCEDURE dbo.TGetCharCounter;
IF OBJECT_ID('dbo.TCHARCOUNTERTABLE', 'U') IS NOT NULL DROP TABLE dbo.TCHARCOUNTERTABLE;
PRINT 'General counter objects removed from ' + DB_NAME();
GO
