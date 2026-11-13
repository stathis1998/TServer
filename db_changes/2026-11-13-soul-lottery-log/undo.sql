-- Removes the soul lottery log and DELETES every draw recorded in it.
-- Run a TMapSvr build from before the soul lottery log first, or the running server will log failed writes.
--   sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -i undo.sql
SET NOCOUNT ON;
IF OBJECT_ID('dbo.TLogSoulLottery', 'P') IS NOT NULL DROP PROCEDURE dbo.TLogSoulLottery;
IF OBJECT_ID('dbo.TSOULLOTTERYLOGTABLE', 'U') IS NOT NULL DROP TABLE dbo.TSOULLOTTERYLOGTABLE;
PRINT 'Soul lottery log objects removed from ' + DB_NAME();
GO
