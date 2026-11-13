-- Adds the per-character level freeze chosen on the portal: table TCHARLEVELFREEZETABLE.
-- A row means that character cannot level past bFreezeLevel. No row means no freeze.
-- Purely additive: changes no existing table or procedure, and the running server ignores it.
-- Apply BEFORE starting a TMapSvr built from commit 'feat: per-character level freeze' or later.
--   sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -i apply.sql
-- Runs as one transaction: any error stops the script and leaves the database unchanged.
:on error exit
SET NOCOUNT ON;
IF DB_NAME() NOT IN ('TGAME_GSP', 'TGAME_GSP_TEST') THROW 50000, 'Run this in TGAME_GSP or TGAME_GSP_TEST, using sqlcmd -d.', 1;
IF OBJECT_ID('dbo.TCHARLEVELFREEZETABLE') IS NOT NULL
	THROW 50000, 'TCHARLEVELFREEZETABLE already exists in this database. Run undo.sql first.', 1;
BEGIN TRANSACTION;
GO

CREATE TABLE dbo.TCHARLEVELFREEZETABLE (
	dwCharID     int      NOT NULL,
	bFreezeLevel tinyint  NOT NULL,
	dUpdate      datetime NOT NULL CONSTRAINT DF_TCHARLEVELFREEZETABLE_dUpdate DEFAULT GETDATE(),
	CONSTRAINT PK_TCHARLEVELFREEZETABLE PRIMARY KEY CLUSTERED (dwCharID),
	CONSTRAINT CK_TCHARLEVELFREEZETABLE_bFreezeLevel CHECK (bFreezeLevel BETWEEN 1 AND 89)
);
GO

COMMIT;
PRINT 'TCHARLEVELFREEZETABLE created in ' + DB_NAME();
GO
