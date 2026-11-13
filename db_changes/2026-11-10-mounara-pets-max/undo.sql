-- Undo: restore Mounara's companions from the backup taken by apply.sql
USE TGAME_GSP;
UPDATE c SET c.bLevel = b.bLevel, c.dwExp = b.dwExp, c.bStatusPoints = b.bStatusPoints,
    c.wSTR = b.wSTR, c.wDEX = b.wDEX, c.wCON = b.wCON, c.wINT = b.wINT, c.wWIS = b.wWIS, c.wMEN = b.wMEN
FROM TCOMPANIONTABLE c
JOIN dbo.TCOMPANIONTABLE_bak_20261110_mounara b ON b.dwCharID = c.dwCharID AND b.bSlot = c.bSlot;
SELECT bSlot, strName, bLevel, dwExp, wSTR, wDEX, wCON, wINT, wWIS, wMEN FROM TCOMPANIONTABLE WHERE dwCharID = 25 ORDER BY bSlot;
