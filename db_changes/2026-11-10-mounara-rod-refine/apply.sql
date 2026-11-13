-- 2026-11-10: Mounara's +22 Cane of Blessed Knowledge, refinements 5/5 -> 4/5
USE TGAME_GSP;
UPDATE TITEMTABLE SET bRefineCur = 4
WHERE dlID = 281474976771407 AND dwOwnerID = 25 AND wItemID = 23905 AND bRefineCur = 5;
SELECT dlID, wItemID, bLevel, bRefineCur FROM TITEMTABLE WHERE dlID = 281474976771407;
