-- Adds the soul lottery log: table TSOULLOTTERYLOGTABLE and procedure TLogSoulLottery.
-- Purely additive: changes no existing table or procedure, and the running server ignores it.
-- Apply BEFORE starting a TMapSvr built from commit 'feat: log every soul lottery draw' or later.
--   sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -i apply.sql
-- Runs as one transaction: any error stops the script and leaves the database unchanged.
--
-- One row per soul lottery draw, written by the map server's DB thread in the order they happen,
-- so dwID orders a character's draws and the portal can find win and lose streaks.
--   bLevel   the character's level when it drew
--   bWon     1 won, 0 lost
--   bDrawn   the number drawn, 1 to 30
--   nMoney   the prize, or the consolation money for a loss, in the smallest coin unit
:on error exit
SET NOCOUNT ON;
IF DB_NAME() NOT IN ('TGAME_GSP', 'TGAME_GSP_TEST') THROW 50000, 'Run this in TGAME_GSP or TGAME_GSP_TEST, using sqlcmd -d.', 1;
IF OBJECT_ID('dbo.TSOULLOTTERYLOGTABLE') IS NOT NULL OR OBJECT_ID('dbo.TLogSoulLottery') IS NOT NULL
	THROW 50000, 'Soul lottery log objects already exist in this database. Run undo.sql first.', 1;
BEGIN TRANSACTION;
GO

CREATE TABLE dbo.TSOULLOTTERYLOGTABLE (
	dwID     bigint   IDENTITY(1,1) NOT NULL,
	dLogDate datetime NOT NULL CONSTRAINT DF_TSOULLOTTERYLOGTABLE_dLogDate DEFAULT GETDATE(),
	dwCharID int      NOT NULL,
	bLevel   tinyint  NOT NULL,
	bWon     tinyint  NOT NULL,
	bDrawn   tinyint  NOT NULL,
	nMoney   bigint   NOT NULL,
	CONSTRAINT PK_TSOULLOTTERYLOGTABLE PRIMARY KEY CLUSTERED (dwID)
);
GO

-- For walking each character's draws in order.
CREATE INDEX IX_TSOULLOTTERYLOGTABLE_dwCharID ON dbo.TSOULLOTTERYLOGTABLE (dwCharID, dwID) INCLUDE (bWon);
GO

CREATE PROCEDURE dbo.TLogSoulLottery
	@dwCharID int,
	@bLevel   tinyint,
	@bWon     tinyint,
	@bDrawn   tinyint,
	@nMoney   bigint
AS
SET NOCOUNT ON;
INSERT INTO dbo.TSOULLOTTERYLOGTABLE (dwCharID, bLevel, bWon, bDrawn, nMoney)
VALUES (@dwCharID, @bLevel, CASE WHEN @bWon <> 0 THEN 1 ELSE 0 END, @bDrawn, @nMoney);
GO

COMMIT;
PRINT 'Soul lottery log objects created in ' + DB_NAME();
GO
