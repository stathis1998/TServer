-- Undo: put 821 "Damage Increased" back to its original state. 823 is already original after
-- apply.sql, so this leaves both skills as they were before the whole Lottery Rush feature.
-- Revert the lottery code too, because it still applies Whip (633) at level 3 for 60 s.
-- Run with: sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -b -i undo.sql
-- Restart TMapSvr afterwards.
SET NOCOUNT ON;
SET XACT_ABORT ON;
BEGIN TRAN;
DELETE FROM TSKILLDATA WHERE wSkillID = 821 AND bAction = 3 AND bType = 1 AND bAttr = 20;
INSERT INTO TSKILLDATA (wSkillID, bAction, bType, bAttr, bExec, bInc, wValue, wValueInc, bCalc)
VALUES (821, 3, 1, 1, 114, 5, 200, 0, 0);
UPDATE TSKILLCHART SET dwDuration = 30000 WHERE wID = 821;
COMMIT;
SELECT wID, szName, dwDuration FROM TSKILLCHART WHERE wID IN (821, 823);
SELECT * FROM TSKILLDATA WHERE wSkillID IN (821, 823) ORDER BY wSkillID, bType, bExec;
