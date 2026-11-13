-- Lottery Rush tuning: 45 s, +10% attack power (105 -> 110), 20% shorter cooldowns (85 -> 80).
-- Speed (Whip 633 at level 4 = 3.30x) and the 45 s duration are set in the TMapSvr code;
-- 821's dwDuration here only keeps the chart consistent.
-- Needs 2026-11-11-lottery-rush-whip applied first.
-- Run with: sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -b -i apply.sql
-- TMapSvr loads the skill charts at startup, so it needs a restart afterwards.
SET NOCOUNT ON;
SET XACT_ABORT ON;
BEGIN TRAN;

IF NOT EXISTS (SELECT 1 FROM TSKILLCHART WHERE wID = 821 AND dwDuration = 60000)
   OR (SELECT COUNT(*) FROM TSKILLDATA WHERE wSkillID = 821) <> 6
   OR (SELECT COUNT(*) FROM TSKILLDATA WHERE wSkillID = 821 AND bAttr = 20 AND bExec IN (7, 9, 17) AND wValue = 105) <> 3
   OR (SELECT COUNT(*) FROM TSKILLDATA WHERE wSkillID = 821 AND bAttr = 20 AND bExec IN (54, 55, 56) AND wValue = 85) <> 3
BEGIN
    RAISERROR('Skill 821 is not the 60 s Lottery Rush version (105/85), nothing changed', 16, 1);
    ROLLBACK;
    RETURN;
END

UPDATE TSKILLDATA SET wValue = 110 WHERE wSkillID = 821 AND bAction = 3 AND bType = 1 AND bAttr = 20 AND bExec IN (7, 9, 17);
UPDATE TSKILLDATA SET wValue = 80  WHERE wSkillID = 821 AND bAction = 3 AND bType = 1 AND bAttr = 20 AND bExec IN (54, 55, 56);
UPDATE TSKILLCHART SET dwDuration = 45000 WHERE wID = 821;

COMMIT;

SELECT wID, szName, dwDuration FROM TSKILLCHART WHERE wID = 821;
SELECT * FROM TSKILLDATA WHERE wSkillID = 821 ORDER BY bExec;
