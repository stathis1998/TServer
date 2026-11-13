-- 2026-11-10: Mounara's (dwCharID 25) companions in slots 0-3 to level 21 with the
-- same stats as slot 4: STR 5, DEX 7, CON 30, INT 35, WIS 5, MEN 5
USE TGAME_GSP;
SELECT * INTO dbo.TCOMPANIONTABLE_bak_20261110_mounara FROM TCOMPANIONTABLE WHERE dwCharID = 25;
UPDATE TCOMPANIONTABLE
SET bLevel = 21, dwExp = 0, bStatusPoints = 0,
    wSTR = 5, wDEX = 7, wCON = 30, wINT = 35, wWIS = 5, wMEN = 5
WHERE dwCharID = 25 AND bSlot IN (0, 1, 2, 3);
SELECT bSlot, dwMonID, strName, bLevel, dwExp, bStatusPoints, wSTR, wDEX, wCON, wINT, wWIS, wMEN, wBonusID
FROM TCOMPANIONTABLE WHERE dwCharID = 25 ORDER BY bSlot;
