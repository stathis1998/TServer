-- Remove Kavliara's (dwCharID 17) stack of 200 First-Class Healing Potions (wItemID 7600)
-- and refund 10 gold (200 x 50,000 copper = 10,000,000 copper = 10 gold).
USE TGAME_GSP;
SET XACT_ABORT ON;
BEGIN TRAN;
IF NOT EXISTS (SELECT 1 FROM TITEMTABLE WHERE dlID=281474976711579 AND dwOwnerID=17 AND bOwnerType=0 AND wItemID=7600 AND bCount=200)
BEGIN
    RAISERROR('Potion stack not found as expected, nothing changed', 16, 1);
    ROLLBACK; RETURN;
END
DELETE FROM TITEMTABLE WHERE dlID=281474976711579 AND dwOwnerID=17 AND wItemID=7600 AND bCount=200;
UPDATE TCHARTABLE SET dwGold = dwGold + 10 WHERE dwCharID=17 AND szNAME='Kavliara';
COMMIT;
SELECT dwCharID, szNAME, dwGold, dwSilver, dwCooper FROM TCHARTABLE WHERE dwCharID=17;
