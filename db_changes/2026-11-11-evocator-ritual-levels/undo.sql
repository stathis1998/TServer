-- Undo: Dark Ritual (623) and Holy Ritual (628) back to the original step 28 (levels 24 / 52 / 80).
-- Run with:  sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -i undo.sql
-- Needs a TMapSvr restart. Characters who already bought a level under the lower requirement keep it.
SET NOCOUNT ON;
SET XACT_ABORT ON;

IF (SELECT COUNT(*) FROM dbo.TSKILLCHART WHERE wID IN (623, 628) AND bLevel = 24 AND bNextLevel = 22 AND bMaxLevel = 3) <> 2
BEGIN
	RAISERROR('Refused: skills 623/628 are not in the applied state (bNextLevel 22).', 16, 1);
	RETURN;
END

BEGIN TRANSACTION;

UPDATE dbo.TSKILLCHART SET bNextLevel = 28 WHERE wID IN (623, 628) AND bNextLevel = 22;

IF @@ROWCOUNT <> 2
BEGIN
	ROLLBACK TRANSACTION;
	RAISERROR('Rolled back: expected exactly 2 rows to change.', 16, 1);
	RETURN;
END

COMMIT TRANSACTION;

SELECT wID, szName, bLevel, bNextLevel, bMaxLevel FROM dbo.TSKILLCHART WHERE wID IN (623, 628);
