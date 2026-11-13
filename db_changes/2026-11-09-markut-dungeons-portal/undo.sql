-- Remove the Markut Dungeons Portal rows added by apply.sql. Restart TMapSvr afterwards.
SET XACT_ABORT ON;
BEGIN TRAN;
DELETE TNPCITEMCHART WHERE wNpcID = 26615 AND dwItemID = 20176;
DELETE TNPCCHART WHERE wID = 26615;
COMMIT;
