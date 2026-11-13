-- Take the 10 gold away from Kavliara again. Run only while TMapSvr is closed.
USE TGAME_GSP;
UPDATE TCHARTABLE SET dwGold = dwGold - 10 WHERE dwCharID = 17 AND szNAME = 'Kavliara' AND dwGold >= 10;
SELECT dwCharID, szNAME, dwGold, dwSilver, dwCooper FROM TCHARTABLE WHERE dwCharID = 17;
