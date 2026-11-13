-- Removes the level freeze table and DELETES every freeze recorded in it.
-- Run a TMapSvr build from before the level freeze first. A newer build keeps working without the
-- table, but logs a failed read at every character load and treats every character as not frozen.
--   sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -i undo.sql
SET NOCOUNT ON;
IF OBJECT_ID('dbo.TCHARLEVELFREEZETABLE', 'U') IS NOT NULL DROP TABLE dbo.TCHARLEVELFREEZETABLE;
PRINT 'TCHARLEVELFREEZETABLE removed from ' + DB_NAME();
GO
