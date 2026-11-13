-- Empties the combat statistics for a new season. Run with every TMapSvr stopped.
-- The save procedures never lower a value, so resetting characters alone would leave these tables
-- holding last season's higher numbers. Add this to the yearly reset.
--   sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -i season-reset.sql
SET NOCOUNT ON;
TRUNCATE TABLE dbo.TCHARMONKILLTABLE;
TRUNCATE TABLE dbo.TCHARSTATTABLE;
IF OBJECT_ID('dbo.TCHARCOUNTERTABLE', 'U') IS NOT NULL TRUNCATE TABLE dbo.TCHARCOUNTERTABLE;
IF OBJECT_ID('dbo.TITEMUPGRADELOGTABLE', 'U') IS NOT NULL TRUNCATE TABLE dbo.TITEMUPGRADELOGTABLE;
IF OBJECT_ID('dbo.TSOULLOTTERYLOGTABLE', 'U') IS NOT NULL TRUNCATE TABLE dbo.TSOULLOTTERYLOGTABLE;
PRINT 'Combat statistics emptied in ' + DB_NAME();
GO
