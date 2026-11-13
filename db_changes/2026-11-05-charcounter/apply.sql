-- Adds general per-character counters: table TCHARCOUNTERTABLE and its procedures.
-- Purely additive: changes no existing table or procedure, and the running server ignores it.
-- Apply BEFORE starting a TMapSvr built from commit 'feat: count upgrades and money' or later.
--   sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -i apply.sql
-- Runs as one transaction: any error stops the script and leaves the database unchanged.
--
-- wStatID values, from enum CHARCOUNTER in Server\TMapSvr\TMapType.h. Never renumber them:
--   0  Item upgrade scrolls that succeeded
--   1  Item upgrade scrolls that failed
--   2  Money received from monsters, in the smallest coin unit
--   3  Money spent, in the smallest coin unit
:on error exit
SET NOCOUNT ON;
IF DB_NAME() NOT IN ('TGAME_GSP', 'TGAME_GSP_TEST') THROW 50000, 'Run this in TGAME_GSP or TGAME_GSP_TEST, using sqlcmd -d.', 1;
IF OBJECT_ID('dbo.TCHARCOUNTERTABLE') IS NOT NULL OR OBJECT_ID('dbo.TSaveCharCounter') IS NOT NULL OR OBJECT_ID('dbo.TGetCharCounter') IS NOT NULL
	THROW 50000, 'General counter objects already exist in this database. Run undo.sql first.', 1;
BEGIN TRANSACTION;
GO

CREATE TABLE dbo.TCHARCOUNTERTABLE (
	dwCharID int      NOT NULL,
	wStatID  smallint NOT NULL,
	nValue   bigint   NOT NULL CONSTRAINT DF_TCHARCOUNTERTABLE_nValue  DEFAULT 0,
	dUpdate  datetime NOT NULL CONSTRAINT DF_TCHARCOUNTERTABLE_dUpdate DEFAULT GETDATE(),
	CONSTRAINT PK_TCHARCOUNTERTABLE PRIMARY KEY CLUSTERED (dwCharID, wStatID)
);
GO

-- For checking a character by hand. The server reads the table with its own SELECT.
CREATE PROCEDURE dbo.TGetCharCounter
	@dwCharID int
AS
SET NOCOUNT ON;
SELECT wStatID, nValue, dUpdate FROM dbo.TCHARCOUNTERTABLE WHERE dwCharID = @dwCharID ORDER BY wStatID;
GO

-- Only ever raises a value, so a late or repeated write cannot lower a counter.
CREATE PROCEDURE dbo.TSaveCharCounter
	@dwCharID int,
	@wStatID  smallint,
	@nValue   bigint
AS
SET NOCOUNT ON;
UPDATE dbo.TCHARCOUNTERTABLE SET
	nValue  = CASE WHEN @nValue > nValue THEN @nValue ELSE nValue END,
	dUpdate = GETDATE()
WHERE dwCharID = @dwCharID AND wStatID = @wStatID;
IF @@ROWCOUNT = 0
	INSERT INTO dbo.TCHARCOUNTERTABLE (dwCharID, wStatID, nValue) VALUES (@dwCharID, @wStatID, @nValue);
GO

COMMIT;
PRINT 'General counter objects created in ' + DB_NAME();
GO
