-- Puts back both databases exactly as they were before the 2026-11-06 season reset.
-- Anything created after the reset (new accounts, characters) is lost.
-- Stop every server first, then:
--   sqlcmd -S "localhost\FOURSTORY" -E -C -b -i undo.sql
USE master;
GO
ALTER DATABASE TGLOBAL_GSP SET SINGLE_USER WITH ROLLBACK IMMEDIATE;
RESTORE DATABASE TGLOBAL_GSP FROM DISK = N'C:\Program Files\Microsoft SQL Server\MSSQL16.FOURSTORY\MSSQL\Backup\TGLOBAL_GSP_before_season_reset_20261106.bak' WITH REPLACE;
ALTER DATABASE TGLOBAL_GSP SET MULTI_USER;
GO
ALTER DATABASE TGAME_GSP SET SINGLE_USER WITH ROLLBACK IMMEDIATE;
RESTORE DATABASE TGAME_GSP FROM DISK = N'C:\Program Files\Microsoft SQL Server\MSSQL16.FOURSTORY\MSSQL\Backup\TGAME_GSP_before_season_reset_20261106.bak' WITH REPLACE;
ALTER DATABASE TGAME_GSP SET MULTI_USER;
GO
