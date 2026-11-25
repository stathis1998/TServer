-- Puts the old loot of boxes 37 and 38 back from $(GLOBAL_DB).dbo.TCASHGAMBLECHART_bak_20261125.
--   sqlcmd -S "localhost\FOURSTORY" -E -C -b -d TGAME_GSP -v GLOBAL_DB=TGLOBAL_GSP -i undo.sql
-- It leaves the bFixedCount column and the view as they are: the old rows get 0 (a random 1 to bCount, as before),
-- and both an old TMapSvr and one built from feature/fire-essence-loot work with them. Restart TMapSvr afterwards.
SET XACT_ABORT ON;
SET NOCOUNT ON;
IF OBJECT_ID('$(GLOBAL_DB).dbo.TCASHGAMBLECHART_bak_20261125') IS NULL
BEGIN
    RAISERROR('No backup table, apply.sql never ran on this database, nothing changed', 16, 1);
    RETURN;
END
BEGIN TRAN;
DELETE FROM $(GLOBAL_DB).dbo.TCASHGAMBLECHART WHERE wGroup IN (37, 38);
EXEC('INSERT INTO $(GLOBAL_DB).dbo.TCASHGAMBLECHART (dwID, wItemID, bCount, dwProb, bLevel, bGLevel, bGradeEffect,
    bMagic1, wValue1, dwTime1, bMagic2, wValue2, dwTime2, bMagic3, wValue3, dwTime3,
    bMagic4, wValue4, dwTime4, bMagic5, wValue5, dwTime5, bMagic6, wValue6, dwTime6,
    wGroup, wUseTime, dwDuraMax, dwDuraCur, bRefineCur, bGem, wMoggItemID)
SELECT dwID, wItemID, bCount, dwProb, bLevel, bGLevel, bGradeEffect,
    bMagic1, wValue1, dwTime1, bMagic2, wValue2, dwTime2, bMagic3, wValue3, dwTime3,
    bMagic4, wValue4, dwTime4, bMagic5, wValue5, dwTime5, bMagic6, wValue6, dwTime6,
    wGroup, wUseTime, dwDuraMax, dwDuraCur, bRefineCur, bGem, wMoggItemID
FROM $(GLOBAL_DB).dbo.TCASHGAMBLECHART_bak_20261125');
COMMIT;
SELECT wGroup, COUNT(*) AS nRows, SUM(dwProb) AS nWeight FROM $(GLOBAL_DB).dbo.TCASHGAMBLECHART WHERE wGroup IN (37, 38) GROUP BY wGroup;
