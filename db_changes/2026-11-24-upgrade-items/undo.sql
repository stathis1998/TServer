-- Undoes apply.sql: Hero's Medal back to 0 medals, items 18045 and 18047 back to their unused stock rows,
-- skills 948 and 949 removed. Characters still holding the potion or tincture keep them, as useless copies
-- of the old items, so take them back first. Restart TMapSvr afterwards.
SET XACT_ABORT ON;
BEGIN TRAN;
UPDATE dbo.TITEMCHART SET wUseValue = 0 WHERE wItemID = 7927 AND bType = 15 AND bKind = 118;
UPDATE dbo.TITEMCHART SET szNAME = 'Serendipity Potion (100%)', wUseValue = 905, fPrice = 0, bIsSell = 4,
    bDropLevel = 1, bIsSpecial = 0, bGroupID = 9, dwCode = 0
WHERE wItemID = 18045 AND bType = 7 AND bKind = 33;
UPDATE dbo.TITEMCHART SET szNAME = 'Survival Tincture', wUseValue = 919, fPrice = 0, bIsSell = 4,
    bDropLevel = 1, bIsSpecial = 0, bGroupID = 9, dwCode = 0
WHERE wItemID = 18047 AND bType = 7 AND bKind = 33;
DELETE FROM dbo.TSKILLMAINTAINTABLE WHERE wSkillID IN (948, 949);
DELETE FROM dbo.TSKILLDATA WHERE wSkillID IN (948, 949);
DELETE FROM dbo.TSKILLCHART WHERE wID IN (948, 949);
COMMIT;
SELECT wItemID, szNAME, wUseValue FROM dbo.TITEMCHART WHERE wItemID IN (7927, 18045, 18047) ORDER BY wItemID;
