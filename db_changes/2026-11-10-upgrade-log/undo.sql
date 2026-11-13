-- Removes the item upgrade log and DELETES every attempt recorded in it.
-- Run a TMapSvr build from before the upgrade log first, or the running server will log failed writes.
--   sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -i undo.sql
SET NOCOUNT ON;
IF OBJECT_ID('dbo.TLogItemUpgrade', 'P') IS NOT NULL DROP PROCEDURE dbo.TLogItemUpgrade;
IF OBJECT_ID('dbo.TITEMUPGRADELOGTABLE', 'U') IS NOT NULL DROP TABLE dbo.TITEMUPGRADELOGTABLE;
PRINT 'Upgrade log objects removed from ' + DB_NAME();
GO
