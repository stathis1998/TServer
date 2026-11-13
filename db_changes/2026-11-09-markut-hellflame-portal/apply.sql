-- Give the server the Markut "Portal to the Hell Flame Labyrinth" NPC (26626), which existed only in the client.
-- Without a TNPCCHART row, OnCS_NPCITEMLIST_REQ returned without replying, so the portal window never opened.
-- wItemID is 0 on purpose: the client chart has 1, but on the server that would require item 1 (Gold) to teleport.
-- Portal 16005 "Hell Flame Labyrinth (Lv 55)" is left out: its spawn lands on map 2200 and map 2202 has no monsters.
SET XACT_ABORT ON;
BEGIN TRAN;

IF EXISTS (SELECT 1 FROM TNPCCHART WHERE wID = 26626)
   OR EXISTS (SELECT 1 FROM TNPCITEMCHART WHERE wNpcID = 26626)
   OR EXISTS (SELECT 1 FROM TPORTALCHART WHERE wPortalID = 20176)
   OR EXISTS (SELECT 1 FROM TSPAWNPOSCHART WHERE wID = 20176)
   OR EXISTS (SELECT 1 FROM TDESTINATIONCHART WHERE wPortalID = 20176)
BEGIN RAISERROR('Rows for NPC 26626 or portal 20176 already exist', 16, 1); ROLLBACK; RETURN; END

INSERT TNPCCHART (wID,NC_szName2,szName,bType,dwClass,bCountryID,wLocalID,bCondition,bDiscountRate,bAddProb,wItemID,wMapID,fPosX,fPosY,fPosZ,szLocal)
VALUES (26626,'_','Portal to the Hell Flame Labyrinth',8,255,1,0,0,0,0,0,0,6626.09,204.01,4774.49,'');

INSERT TPORTALCHART (wPortalID,bCountry,wLocalID,wSpawnID,bCondition)
VALUES (20176,1,0,20176,1);

INSERT TSPAWNPOSCHART (wID,wMapID,fPosX,fPosY,fPosZ,bType)
VALUES (20176,0,6629,204.01,4782,0);

INSERT TDESTINATIONCHART (wPortalID,wDestID,dwPrice,bEnable,bCondition1,dwConditionID1,bCondition2,dwConditionID2,bCondition3,dwConditionID3)
VALUES (20176,16001,104263,1,0,0,0,0,0,0),  -- Hell Flame Labyrinth
       (20176,16003,157188,1,0,0,0,0,0,0);  -- Marduk's Nest

INSERT TNPCITEMCHART (wNpcID,dwItemID)
VALUES (26626,20176);

COMMIT;
