-- Soul lottery farming buff: repurpose the unused skill 823 "Speed Increased"
-- as "Lottery Rush", which TMapSvr applies after every soul lottery (win or lose).
--   60 s (was 30 s), move speed 3.25x (wValue 200 -> 225, client adds value/100 to 1.0),
--   +5% physical, ranged and magic attack power (copied from Courageousness Potion 1011).
-- Saddled mount + whip is 3.0x; TMapSvr kicks anything reporting above 3.40x.
-- Run with: sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -b -i apply.sql
-- TMapSvr loads the skill charts at startup, so it needs a restart afterwards.
SET NOCOUNT ON;
SET XACT_ABORT ON;
BEGIN TRAN;

IF NOT EXISTS (SELECT 1 FROM TSKILLCHART WHERE wID = 823 AND szName = 'Speed Increased' AND dwDuration = 30000)
   OR (SELECT COUNT(*) FROM TSKILLDATA WHERE wSkillID = 823) <> 1
   OR NOT EXISTS (SELECT 1 FROM TSKILLDATA WHERE wSkillID = 823 AND bAction = 3 AND bType = 6 AND bAttr = 1
                  AND bExec = 23 AND bInc = 1 AND wValue = 200 AND wValueInc = 0 AND bCalc = 0)
BEGIN
    RAISERROR('Skill 823 is not in the expected original state, nothing changed', 16, 1);
    ROLLBACK;
    RETURN;
END

UPDATE TSKILLCHART SET szName = 'Lottery Rush', dwDuration = 60000 WHERE wID = 823;

UPDATE TSKILLDATA SET wValue = 225
WHERE wSkillID = 823 AND bAction = 3 AND bType = 6 AND bAttr = 1 AND bExec = 23;

-- SA_BUFF (3), SDT_ABILITY (1), MTYPE_PAP (7) / MTYPE_LAP (9) / MTYPE_MAP (17), SVI_PRECENT (5), 105%
INSERT INTO TSKILLDATA (wSkillID, bAction, bType, bAttr, bExec, bInc, wValue, wValueInc, bCalc)
VALUES (823, 3, 1, 0, 7, 5, 105, 0, 0),
       (823, 3, 1, 0, 9, 5, 105, 0, 0),
       (823, 3, 1, 0, 17, 5, 105, 0, 0);

COMMIT;

SELECT wID, szName, dwDuration FROM TSKILLCHART WHERE wID = 823;
SELECT * FROM TSKILLDATA WHERE wSkillID = 823 ORDER BY bType, bExec;
