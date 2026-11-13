-- Puts the Large Crystal money rewards back to their values before 2026-11-09.
-- Run with: sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -b -i undo.sql
-- Then restart TMapSvr and put TRpsGame.tcd.original back as Tcd\TRpsGame.tcd.
SET NOCOUNT ON;
SET XACT_ABORT ON;
BEGIN TRAN;

UPDATE TRPSGAMECHART
SET dwRewardMoney = CASE bWinCount
        WHEN 1 THEN 10000
        WHEN 2 THEN 20000
        WHEN 3 THEN 40000
        WHEN 4 THEN 80000
        WHEN 5 THEN 160000
        WHEN 6 THEN 320000
        WHEN 7 THEN 640000
        ELSE 1280000
    END
WHERE bType = 3 AND bWinCount BETWEEN 1 AND 15;

IF @@ROWCOUNT <> 15
BEGIN
    RAISERROR('Expected to update 15 rows, nothing changed', 16, 1);
    ROLLBACK;
    RETURN;
END

COMMIT;
