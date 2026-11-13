-- Add 10 gold to Kavliara (dwCharID 17). Run only while TMapSvr is closed,
-- otherwise the server overwrites dwGold when it saves the character.
USE TGAME_GSP;
SET XACT_ABORT ON;
BEGIN TRAN;
UPDATE TCHARTABLE SET dwGold = dwGold + 10 WHERE dwCharID = 17 AND szNAME = 'Kavliara';
IF @@ROWCOUNT <> 1
BEGIN
    RAISERROR('Kavliara not found as expected, nothing changed', 16, 1);
    ROLLBACK; RETURN;
END
COMMIT;
SELECT dwCharID, szNAME, dwGold, dwSilver, dwCooper FROM TCHARTABLE WHERE dwCharID = 17;
