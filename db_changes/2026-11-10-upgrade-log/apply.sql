-- Adds the item upgrade log: table TITEMUPGRADELOGTABLE and procedure TLogItemUpgrade.
-- Purely additive: changes no existing table or procedure, and the running server ignores it.
-- Apply BEFORE starting a TMapSvr built from commit 'feat: log every item upgrade attempt' or later.
--   sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -i apply.sql
-- Runs as one transaction: any error stops the script and leaves the database unchanged.
--
-- One row per upgrade scroll or gemstone roll, written by the map server's DB thread.
-- Values come from the UPGRADELOG_* enums in Server\TMapSvr\TMapType.h. Never renumber them:
--   bKind     1 upgrade scroll, 2 gemstone
--   bResult   1 success
--             2 failed, item destroyed
--             3 failed, item kept but lost levels (gem: gem level dropped)
--             4 failed, item kept with no loss (a tincture or gem protection saved it,
--               or a gem failed at gem level 0 with nothing to lose)
--   bProtect  0 none, 1 Survival Tincture, 2 Medium Tincture, 3 Greater Tincture
--             (the tincture the server applies to a failure at this level; always 0 for gems)
--   bLevelBefore / bLevelAfter   the +level for scrolls, the gem level for gems.
--             bLevelAfter is NULL when the item was destroyed.
--   bChance   the final success chance in percent, after Serendipity and NPC bonuses
--   nFee      money paid to the NPC, in the smallest coin unit
:on error exit
SET NOCOUNT ON;
IF DB_NAME() NOT IN ('TGAME_GSP', 'TGAME_GSP_TEST') THROW 50000, 'Run this in TGAME_GSP or TGAME_GSP_TEST, using sqlcmd -d.', 1;
IF OBJECT_ID('dbo.TITEMUPGRADELOGTABLE') IS NOT NULL OR OBJECT_ID('dbo.TLogItemUpgrade') IS NOT NULL
	THROW 50000, 'Upgrade log objects already exist in this database. Run undo.sql first.', 1;
BEGIN TRANSACTION;
GO

CREATE TABLE dbo.TITEMUPGRADELOGTABLE (
	dwID         bigint   IDENTITY(1,1) NOT NULL,
	dLogDate     datetime NOT NULL CONSTRAINT DF_TITEMUPGRADELOGTABLE_dLogDate DEFAULT GETDATE(),
	dwCharID     int      NOT NULL,
	bKind        tinyint  NOT NULL,
	bResult      tinyint  NOT NULL,
	wItemID      int      NOT NULL,
	wScrollID    int      NOT NULL,
	bLevelBefore tinyint  NOT NULL,
	bLevelAfter  tinyint  NULL,
	bChance      tinyint  NOT NULL,
	bProtect     tinyint  NOT NULL,
	wMapID       int      NOT NULL,
	wNpcID       int      NOT NULL,
	nFee         bigint   NOT NULL,
	CONSTRAINT PK_TITEMUPGRADELOGTABLE PRIMARY KEY CLUSTERED (dwID)
);
GO

-- For a character's own history on the portal.
CREATE INDEX IX_TITEMUPGRADELOGTABLE_dwCharID ON dbo.TITEMUPGRADELOGTABLE (dwCharID, dLogDate);
GO

CREATE PROCEDURE dbo.TLogItemUpgrade
	@dwCharID     int,
	@bKind        tinyint,
	@bResult      tinyint,
	@wItemID      int,
	@wScrollID    int,
	@bLevelBefore tinyint,
	@bLevelAfter  tinyint,
	@bChance      tinyint,
	@bProtect     tinyint,
	@wMapID       int,
	@wNpcID       int,
	@nFee         bigint
AS
SET NOCOUNT ON;
INSERT INTO dbo.TITEMUPGRADELOGTABLE
	(dwCharID, bKind, bResult, wItemID, wScrollID, bLevelBefore, bLevelAfter, bChance, bProtect, wMapID, wNpcID, nFee)
VALUES
	(@dwCharID, @bKind, @bResult, @wItemID, @wScrollID, @bLevelBefore,
	 CASE WHEN @bResult = 2 THEN NULL ELSE @bLevelAfter END,
	 @bChance, @bProtect, @wMapID, @wNpcID, @nFee);
GO

COMMIT;
PRINT 'Upgrade log objects created in ' + DB_NAME();
GO
