-- Give the server the visible Markut "Dungeons Portal" NPC (26615) and point it at portal 20176,
-- so its menu offers Hell Flame Labyrinth and Marduk's Nest.
-- NPC 26626, added earlier the same day, is hidden by the client because its chart row requires item 1 (Gold), which nobody can hold.
-- Needs db_changes/2026-11-09-markut-hellflame-portal/apply.sql to have run first, because that adds portal 20176.
SET XACT_ABORT ON;
BEGIN TRAN;

IF NOT EXISTS (SELECT 1 FROM TPORTALCHART WHERE wPortalID = 20176)
BEGIN RAISERROR('Portal 20176 does not exist', 16, 1); ROLLBACK; RETURN; END

IF EXISTS (SELECT 1 FROM TNPCCHART WHERE wID = 26615)
   OR EXISTS (SELECT 1 FROM TNPCITEMCHART WHERE wNpcID = 26615)
BEGIN RAISERROR('Rows for NPC 26615 already exist', 16, 1); ROLLBACK; RETURN; END

INSERT TNPCCHART (wID,NC_szName2,szName,bType,dwClass,bCountryID,wLocalID,bCondition,bDiscountRate,bAddProb,wItemID,wMapID,fPosX,fPosY,fPosZ,szLocal)
SELECT 26615,'_','Dungeons Portal',8,255,1,0,0,0,0,0,0,n.fPosX - 5.0,n.fPosY,n.fPosZ + 12.0,''
FROM TNPCCHART n WHERE n.wID = 26626;

INSERT TNPCITEMCHART (wNpcID,dwItemID)
VALUES (26615,20176);

COMMIT;
