-- Lottery Rush without client files.
-- Move speed now comes from the Sorcerer's Whip (633) at level 3 (3.20x), which every client
-- already has. Attack power and cooldowns move to the unused skill 821 "Damage Increased",
-- which has no speed effect in the client. 823 goes back to its original state, otherwise
-- old clients would cap the speed at 823's 3.0x (the client keeps the slowest speed buff).
--
-- 821's rows use bAttr 20, which no other skill uses. Buffs collide when bType, bAttr and bExec
-- match: the lottery buff would erase a Courageousness Potion (bAttr 0, same +5%), or be rejected
-- whole by a stronger one such as Attack-power chocolate. bAttr isn't used when stats are calculated.
--
-- Needs 2026-11-11-lottery-farm-buff and 2026-11-11-lottery-rush-cooldown applied first.
-- Run with: sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -b -i apply.sql
-- TMapSvr loads the skill charts at startup, so it needs a restart afterwards.
SET NOCOUNT ON;
SET XACT_ABORT ON;
BEGIN TRAN;

IF NOT EXISTS (SELECT 1 FROM TSKILLCHART WHERE wID = 823 AND szName = 'Lottery Rush' AND dwDuration = 60000)
   OR (SELECT COUNT(*) FROM TSKILLDATA WHERE wSkillID = 823) <> 7
   OR NOT EXISTS (SELECT 1 FROM TSKILLCHART WHERE wID = 821 AND szName = 'Damage Increased' AND dwDuration = 30000)
   OR (SELECT COUNT(*) FROM TSKILLDATA WHERE wSkillID = 821) <> 1
   OR NOT EXISTS (SELECT 1 FROM TSKILLDATA WHERE wSkillID = 821 AND bAction = 3 AND bType = 1 AND bAttr = 1
                  AND bExec = 114 AND bInc = 5 AND wValue = 200 AND wValueInc = 0 AND bCalc = 0)
BEGIN
    RAISERROR('Skills 821/823 are not in the expected state, nothing changed', 16, 1);
    ROLLBACK;
    RETURN;
END

-- 823 back to the original "Speed Increased"
DELETE FROM TSKILLDATA WHERE wSkillID = 823 AND bAction = 3 AND bType = 1 AND bAttr = 0 AND bExec IN (7, 9, 17, 54, 55, 56);
UPDATE TSKILLDATA SET wValue = 200 WHERE wSkillID = 823 AND bAction = 3 AND bType = 6 AND bAttr = 1 AND bExec = 23;
UPDATE TSKILLCHART SET szName = 'Speed Increased', dwDuration = 30000 WHERE wID = 823;

-- 821 becomes the attack power and cooldown part. Its old effect 114 is MTYPE_COUNT, which does nothing.
DELETE FROM TSKILLDATA WHERE wSkillID = 821 AND bAction = 3 AND bType = 1 AND bAttr = 1 AND bExec = 114;
UPDATE TSKILLCHART SET dwDuration = 60000 WHERE wID = 821;
-- SA_BUFF (3), SDT_ABILITY (1), bAttr 20, SVI_PRECENT (5):
--   MTYPE_PAP/LAP/MAP (7/9/17) at 105 = +5% attack power
--   MTYPE_PAS/LAS/MPAS (54/55/56) at 85 = cooldown rate 85, 15% shorter cooldowns
INSERT INTO TSKILLDATA (wSkillID, bAction, bType, bAttr, bExec, bInc, wValue, wValueInc, bCalc)
VALUES (821, 3, 1, 20, 7, 5, 105, 0, 0),
       (821, 3, 1, 20, 9, 5, 105, 0, 0),
       (821, 3, 1, 20, 17, 5, 105, 0, 0),
       (821, 3, 1, 20, 54, 5, 85, 0, 0),
       (821, 3, 1, 20, 55, 5, 85, 0, 0),
       (821, 3, 1, 20, 56, 5, 85, 0, 0);

COMMIT;

SELECT wID, szName, dwDuration FROM TSKILLCHART WHERE wID IN (821, 823);
SELECT * FROM TSKILLDATA WHERE wSkillID IN (821, 823) ORDER BY wSkillID, bType, bExec;
