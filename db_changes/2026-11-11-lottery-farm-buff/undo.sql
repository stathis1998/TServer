-- Undo the soul lottery farming buff: restore skill 823 "Speed Increased" (30 s, 3.0x speed, no attack bonus).
-- Run with: sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -b -i undo.sql
-- Restart TMapSvr afterwards. Also put back TSkill.tcd.original and TSkillFunction.tcd.original.
SET NOCOUNT ON;
SET XACT_ABORT ON;
BEGIN TRAN;

DELETE FROM TSKILLDATA WHERE wSkillID = 823 AND bAction = 3 AND bType = 1 AND bAttr = 0 AND bExec IN (7, 9, 17);

UPDATE TSKILLDATA SET wValue = 200
WHERE wSkillID = 823 AND bAction = 3 AND bType = 6 AND bAttr = 1 AND bExec = 23;

UPDATE TSKILLCHART SET szName = 'Speed Increased', dwDuration = 30000 WHERE wID = 823;

COMMIT;

SELECT wID, szName, dwDuration FROM TSKILLCHART WHERE wID = 823;
SELECT * FROM TSKILLDATA WHERE wSkillID = 823 ORDER BY bType, bExec;
