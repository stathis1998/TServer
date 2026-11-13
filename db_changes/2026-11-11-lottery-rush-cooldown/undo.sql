-- Undo: remove the cooldown rows from Lottery Rush (skill 823). Restart TMapSvr afterwards,
-- and put back TSkillFunction.tcd.original from this folder.
-- Run with: sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -b -i undo.sql
SET NOCOUNT ON;
SET XACT_ABORT ON;
BEGIN TRAN;
DELETE FROM TSKILLDATA WHERE wSkillID = 823 AND bAction = 3 AND bType = 1 AND bAttr = 0 AND bExec IN (54, 55, 56);
COMMIT;
SELECT * FROM TSKILLDATA WHERE wSkillID = 823 ORDER BY bType, bExec;
