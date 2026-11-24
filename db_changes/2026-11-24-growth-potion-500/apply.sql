-- 500% Growth Potion (1 hour): item 30309 (an unused 300% Growth Potion (1 hour)) gives +400% kill EXP instead of +200%.
-- The server adds wUseValue percent to monster EXP (CTPlayer::IsExpBenefit), so 400 means 5 times the normal EXP.
-- TMapSvr reads TITEMCHART at start, so restart it afterwards. No rebuild. Safe to run twice. Undo with undo.sql.
SET XACT_ABORT ON;
BEGIN TRAN;
UPDATE dbo.TITEMCHART SET wUseValue = 400, szNAME = '500% Growth Potion (1 hour)'
WHERE wItemID = 30309 AND bType = 7 AND bKind = 56 AND wUseTime = 1 AND wUseValue IN (200, 400);
IF @@ROWCOUNT <> 1
BEGIN
    RAISERROR('Item 30309 is not the potion this script expects, nothing changed', 16, 1);
    ROLLBACK; RETURN;
END
COMMIT;
SELECT wItemID, bType, bKind, szNAME, wUseValue, wUseTime, bUseType FROM dbo.TITEMCHART WHERE wItemID = 30309;
