-- New loot for Normal Fire Essence (18075, box 37) and Marduk's Fire Essence (18076, box 38), 2026-11-25.
-- Both boxes get the same 13 rows. dwProb is a weight out of the box's total (100 here), so it reads as a percent.
--
-- Run it in the game database and name the global database, for example:
--   sqlcmd -S "localhost\FOURSTORY" -E -C -b -d TGAME_GSP -v GLOBAL_DB=TGLOBAL_GSP -i apply.sql
--
-- Needs 2026-11-24-growth-potion-500 and 2026-11-24-upgrade-items first (it checks).
-- Adds TCASHGAMBLECHART.bFixedCount (default 0) and puts it in TVIEW_CASHGAMBLECHART. A row with 1 always gives
-- bCount; every other box keeps the old random 1 to bCount. The new rows use 1. A TMapSvr built from
-- feature/fire-essence-loot needs the column, so run this script before starting it; an older TMapSvr ignores
-- the column and gives a random 1 to bCount.
-- The old rows of boxes 37 and 38 are kept in $(GLOBAL_DB).dbo.TCASHGAMBLECHART_bak_20261125 for undo.sql.
-- TMapSvr reads the boxes at start, so restart it afterwards. Safe to run twice.
SET XACT_ABORT ON;
SET NOCOUNT ON;
BEGIN TRAN;

-- The reward items must already be what this table promises
IF (SELECT COUNT(*) FROM dbo.TITEMCHART WHERE
        (wItemID = 18047 AND szNAME = 'Epic Survival Tincture' AND wUseValue = 949)
     OR (wItemID = 18045 AND szNAME = 'Serendipity Potion (400%)' AND wUseValue = 948)
     OR (wItemID = 7927 AND bKind = 118 AND wUseValue = 10000)
     OR (wItemID = 30309 AND bKind = 56 AND wUseValue = 400)
     OR (wItemID = 7860 AND bKind = 56 AND wUseValue = 200 AND wUseTime = 1)
     OR (wItemID = 18075 AND bKind = 78 AND wUseValue = 37)
     OR (wItemID = 18076 AND bKind = 78 AND wUseValue = 38)) <> 7
BEGIN
    RAISERROR('Run 2026-11-24-growth-potion-500 and 2026-11-24-upgrade-items first (or an item is not what this script expects), nothing changed', 16, 1);
    ROLLBACK; RETURN;
END

-- Keep the old loot once, before anything changes
IF OBJECT_ID('$(GLOBAL_DB).dbo.TCASHGAMBLECHART_bak_20261125') IS NULL
    EXEC('SELECT dwID, wItemID, bCount, dwProb, bLevel, bGLevel, bGradeEffect, bMagic1, wValue1, dwTime1, bMagic2, wValue2, dwTime2,
            bMagic3, wValue3, dwTime3, bMagic4, wValue4, dwTime4, bMagic5, wValue5, dwTime5, bMagic6, wValue6, dwTime6,
            wGroup, wUseTime, dwDuraMax, dwDuraCur, bRefineCur, bGem, wMoggItemID
          INTO $(GLOBAL_DB).dbo.TCASHGAMBLECHART_bak_20261125
          FROM $(GLOBAL_DB).dbo.TCASHGAMBLECHART WHERE wGroup IN (37, 38)');

IF COL_LENGTH('$(GLOBAL_DB).dbo.TCASHGAMBLECHART', 'bFixedCount') IS NULL
    ALTER TABLE $(GLOBAL_DB).dbo.TCASHGAMBLECHART
        ADD bFixedCount tinyint NOT NULL CONSTRAINT DF_TCASHGAMBLECHART_bFixedCount DEFAULT 0;

EXEC('ALTER VIEW dbo.TVIEW_CASHGAMBLECHART AS
SELECT  dwID, wGroup, wUseTime, wItemID, bCount, dwProb, bLevel, bGLevel, bGradeEffect,
	dwDuraMax, dwDuraCur, bRefineCur,
	bMagic1, bMagic2, bMagic3, bMagic4, bMagic5, bMagic6,
	wValue1, wValue2, wValue3, wValue4, wValue5, wValue6,
	dwTime1, dwTime2, dwTime3, dwTime4, dwTime5, dwTime6,
	bGem, wMoggItemID, bFixedCount
FROM      $(GLOBAL_DB).dbo.TCASHGAMBLECHART');

-- The new loot, the same for both boxes
EXEC('
DECLARE @loot TABLE (n int, wItemID smallint, bCount tinyint, dwProb int);
INSERT INTO @loot VALUES
    ( 1, 18047,   1,  5),  -- Epic Survival Tincture
    ( 2, 18045,   1,  5),  -- Serendipity Potion (400%)
    ( 3,  7927,   1, 10),  -- Hero''s Medal (10,000 medals)
    ( 4,  7860,   5, 10),  -- 300% Growth Potion (1 hour)
    ( 5, 30309,   1,  5),  -- 500% Growth Potion (1 hour)
    ( 6,  7921,   5,  5),  -- Greater Survival Tincture (Tranus)
    ( 7,  7920,  25,  5),  -- Medium Survival Tincture (Tranus)
    ( 8,  7654, 100,  5),  -- Survival Tincture (Tranus, Surita)
    ( 9,  7863,   5,  5),  -- Serendipity Potion (300%) (Tranus)
    (10,  7679,  25,  5),  -- Serendipity Potion (200%) (Tranus)
    (11,  7609, 100,  5),  -- Serendipity Potion (100%) (Tranus, Surita)
    (12,  7630, 100, 10),  -- Arcane Reagent (Surita)
    (13, 18396,   1, 25);  -- GM Coupon

DELETE FROM $(GLOBAL_DB).dbo.TCASHGAMBLECHART WHERE wGroup IN (37, 38);

DECLARE @base int = (SELECT ISNULL(MAX(dwID), 0) FROM $(GLOBAL_DB).dbo.TCASHGAMBLECHART);
INSERT INTO $(GLOBAL_DB).dbo.TCASHGAMBLECHART (dwID, wItemID, bCount, dwProb, bLevel, bGLevel, bGradeEffect,
    bMagic1, wValue1, dwTime1, bMagic2, wValue2, dwTime2, bMagic3, wValue3, dwTime3,
    bMagic4, wValue4, dwTime4, bMagic5, wValue5, dwTime5, bMagic6, wValue6, dwTime6,
    wGroup, wUseTime, dwDuraMax, dwDuraCur, bRefineCur, bGem, wMoggItemID, bFixedCount)
SELECT @base + (g.wGroup - 37) * 13 + l.n, l.wItemID, l.bCount, l.dwProb, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    g.wGroup, 0, 0, 0, 0, 0, 0, 1
FROM @loot l CROSS JOIN (VALUES (37), (38)) g(wGroup);

IF (SELECT COUNT(*) FROM $(GLOBAL_DB).dbo.TCASHGAMBLECHART WHERE wGroup IN (37, 38)) <> 26
   OR (SELECT SUM(dwProb) FROM $(GLOBAL_DB).dbo.TCASHGAMBLECHART WHERE wGroup = 37) <> 100
   OR (SELECT SUM(dwProb) FROM $(GLOBAL_DB).dbo.TCASHGAMBLECHART WHERE wGroup = 38) <> 100
    THROW 50000, ''The new loot did not come out as 2 x 13 rows of 100 in total, nothing changed'', 1;
');

COMMIT;

EXEC('SELECT g.wGroup, g.dwID, g.wItemID, i.szNAME, g.bCount, g.bFixedCount, g.dwProb
FROM dbo.TVIEW_CASHGAMBLECHART g LEFT JOIN dbo.TITEMCHART i ON i.wItemID = g.wItemID
WHERE g.wGroup IN (37, 38) ORDER BY g.wGroup, g.dwID');
