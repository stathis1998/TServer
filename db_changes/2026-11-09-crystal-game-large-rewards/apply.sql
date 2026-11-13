-- Crystal Game: raise the Large Crystal money rewards so they match Small and
-- Normal (best-play return 97.1% of the crystal's 50s price, up from 9.7%).
-- Item rewards and win chances are not touched.
-- Run with: sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -b -i apply.sql
-- TMapSvr loads this table at startup, so it needs a restart afterwards.
SET NOCOUNT ON;
SET XACT_ABORT ON;
BEGIN TRAN;

IF (SELECT COUNT(*) FROM TRPSGAMECHART WHERE bType = 3 AND (
        (bWinCount = 1 AND dwRewardMoney = 10000) OR
        (bWinCount = 2 AND dwRewardMoney = 20000) OR
        (bWinCount = 3 AND dwRewardMoney = 40000) OR
        (bWinCount = 4 AND dwRewardMoney = 80000) OR
        (bWinCount = 5 AND dwRewardMoney = 160000) OR
        (bWinCount = 6 AND dwRewardMoney = 320000) OR
        (bWinCount = 7 AND dwRewardMoney = 640000) OR
        (bWinCount BETWEEN 8 AND 15 AND dwRewardMoney = 1280000))) <> 15
BEGIN
    RAISERROR('Large Crystal rewards are not the expected old values, nothing changed', 16, 1);
    ROLLBACK;
    RETURN;
END

UPDATE TRPSGAMECHART
SET dwRewardMoney = CASE bWinCount
        WHEN 1 THEN 100000
        WHEN 2 THEN 200000
        WHEN 3 THEN 400000
        WHEN 4 THEN 800000
        ELSE 1600000
    END
WHERE bType = 3 AND bWinCount BETWEEN 1 AND 15;

IF @@ROWCOUNT <> 15
BEGIN
    RAISERROR('Expected to update 15 rows, nothing changed', 16, 1);
    ROLLBACK;
    RETURN;
END

COMMIT;
