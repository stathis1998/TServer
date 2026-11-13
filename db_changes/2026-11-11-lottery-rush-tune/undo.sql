-- Undo the tuning: back to 60 s, +5% attack power, 15% shorter cooldowns. Restart TMapSvr afterwards
-- (and revert the code change that set Whip level 4 and 45 s).
-- Run with: sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -b -i undo.sql
SET NOCOUNT ON;
SET XACT_ABORT ON;
BEGIN TRAN;
UPDATE TSKILLDATA SET wValue = 105 WHERE wSkillID = 821 AND bAction = 3 AND bType = 1 AND bAttr = 20 AND bExec IN (7, 9, 17);
UPDATE TSKILLDATA SET wValue = 85  WHERE wSkillID = 821 AND bAction = 3 AND bType = 1 AND bAttr = 20 AND bExec IN (54, 55, 56);
UPDATE TSKILLCHART SET dwDuration = 60000 WHERE wID = 821;
COMMIT;
SELECT wID, szName, dwDuration FROM TSKILLCHART WHERE wID = 821;
SELECT * FROM TSKILLDATA WHERE wSkillID = 821 ORDER BY bExec;
