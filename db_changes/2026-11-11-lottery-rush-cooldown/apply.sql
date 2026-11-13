-- Lottery Rush (skill 823): add 15% shorter skill cooldowns.
-- The server's reuse delay is (base + attack speed) x rate / 100, and the rate starts at 100.
-- MTYPE_PAS (54) / MTYPE_LAS (55) / MTYPE_MPAS (56) with SVI_PRECENT (5) at 85 make it 85.
-- Same kind of rows as Commanding Shout (3002, 95) and Lightning Magic (432, 94).
-- Needs 2026-11-11-lottery-farm-buff first.
-- Run with: sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -b -i apply.sql
-- TMapSvr loads the skill charts at startup, so it needs a restart afterwards.
SET NOCOUNT ON;
SET XACT_ABORT ON;
BEGIN TRAN;

IF NOT EXISTS (SELECT 1 FROM TSKILLCHART WHERE wID = 823 AND szName = 'Lottery Rush')
   OR (SELECT COUNT(*) FROM TSKILLDATA WHERE wSkillID = 823) <> 4
   OR EXISTS (SELECT 1 FROM TSKILLDATA WHERE wSkillID = 823 AND bExec IN (54, 55, 56))
BEGIN
    RAISERROR('Skill 823 is not Lottery Rush with its 4 rows, or already has cooldown rows; nothing changed', 16, 1);
    ROLLBACK;
    RETURN;
END

INSERT INTO TSKILLDATA (wSkillID, bAction, bType, bAttr, bExec, bInc, wValue, wValueInc, bCalc)
VALUES (823, 3, 1, 0, 54, 5, 85, 0, 0),
       (823, 3, 1, 0, 55, 5, 85, 0, 0),
       (823, 3, 1, 0, 56, 5, 85, 0, 0);

COMMIT;

SELECT * FROM TSKILLDATA WHERE wSkillID = 823 ORDER BY bType, bExec;
