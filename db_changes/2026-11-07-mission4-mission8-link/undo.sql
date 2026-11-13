-- Restore the two Mission 4 camp -> Mission 8 links exactly as they were.
SET XACT_ABORT ON;
BEGIN TRAN;
INSERT TDESTINATIONCHART (wPortalID,wDestID,dwPrice,bEnable,bCondition1,dwConditionID1,bCondition2,dwConditionID2,bCondition3,dwConditionID3)
SELECT v.p, 9707, 0, 1, 0, 0, 0, 0, 0, 0 FROM (VALUES (9916),(9917)) v(p)
WHERE NOT EXISTS (SELECT 1 FROM TDESTINATIONCHART d WHERE d.wPortalID = v.p AND d.wDestID = 9707);
COMMIT;
