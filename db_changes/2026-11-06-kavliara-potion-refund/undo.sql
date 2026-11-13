-- Put the 200 First-Class Healing Potions back and take the 10 gold away again.
USE TGAME_GSP;
SET XACT_ABORT ON;
BEGIN TRAN;
INSERT INTO TITEMTABLE (dlID,bStorageType,dwStorageID,bOwnerType,dwOwnerID,bItemID,wItemID,bLevel,bCount,bGLevel,dwDuraMax,dwDuraCur,bRefineCur,dEndTime,bGradeEffect,bMagic1,bMagic2,bMagic3,bMagic4,bMagic5,bMagic6,wValue1,wValue2,wValue3,wValue4,wValue5,wValue6,dwTime1,dwTime2,dwTime3,dwTime4,dwTime5,dwTime6,bGem,wMoggItemID)
VALUES (281474976711579,0,255,0,17,1,7600,0,200,0,0,0,0,'1900-01-01 00:00:00',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
UPDATE TCHARTABLE SET dwGold = dwGold - 10 WHERE dwCharID=17 AND dwGold >= 10;
COMMIT;
