-- Removes the combat statistics tables and procedures, and DELETES every recorded statistic.
-- Stop any TMapSvr built from the feature/char-combat-stats branch first.
--   sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -i undo.sql
SET NOCOUNT ON;
IF OBJECT_ID('dbo.TSaveCharMonKill', 'P') IS NOT NULL DROP PROCEDURE dbo.TSaveCharMonKill;
IF OBJECT_ID('dbo.TSaveCharStat', 'P') IS NOT NULL DROP PROCEDURE dbo.TSaveCharStat;
IF OBJECT_ID('dbo.TGetCharMonKill', 'P') IS NOT NULL DROP PROCEDURE dbo.TGetCharMonKill;
IF OBJECT_ID('dbo.TGetCharStat', 'P') IS NOT NULL DROP PROCEDURE dbo.TGetCharStat;
IF OBJECT_ID('dbo.TCHARMONKILLTABLE', 'U') IS NOT NULL DROP TABLE dbo.TCHARMONKILLTABLE;
IF OBJECT_ID('dbo.TCHARSTATTABLE', 'U') IS NOT NULL DROP TABLE dbo.TCHARSTATTABLE;
PRINT 'Combat statistics objects removed from ' + DB_NAME();
GO
