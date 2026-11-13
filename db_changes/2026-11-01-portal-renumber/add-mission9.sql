-- Adds Mission 9 to the Magnaktia hub, with the same conditions as the Markut hub's entry. Needs a TMapSvr restart.
SET NOCOUNT ON; SET XACT_ABORT ON;
IF EXISTS(SELECT 1 FROM TDESTINATIONCHART WHERE wPortalID=2002 AND wDestID=9508) THROW 50000,'Already applied',1;
IF NOT EXISTS(SELECT 1 FROM TPORTALCHART p JOIN TSPAWNPOSCHART s ON s.wID=p.wSpawnID WHERE p.wPortalID=9508 AND s.wMapID=703) THROW 50000,'9508 is not on map 703',1;
INSERT TDESTINATIONCHART (wPortalID,wDestID,dwPrice,bEnable,bCondition1,dwConditionID1,bCondition2,dwConditionID2,bCondition3,dwConditionID3)
SELECT 2002,9508,dwPrice,bEnable,bCondition1,dwConditionID1,bCondition2,dwConditionID2,bCondition3,dwConditionID3 FROM TDESTINATIONCHART WHERE wPortalID=2102 AND wDestID=9708;
IF @@ROWCOUNT<>1 THROW 50000,'Expected to add one row',1;
PRINT 'Added 2002 to 9508 in '+DB_NAME();
