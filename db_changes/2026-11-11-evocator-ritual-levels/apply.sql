-- Apply: Evocator Dark Ritual (623) and Holy Ritual (628) upgrade at levels 24 / 46 / 68
-- instead of 24 / 52 / 80, matching what the client tooltip already shows (client TSkill.tcd step 22).
-- Run with:  sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -i apply.sql
-- Needs a TMapSvr restart (skill charts load at startup). No client files change.
SET NOCOUNT ON;
SET XACT_ABORT ON;

IF (SELECT COUNT(*) FROM dbo.TSKILLCHART WHERE wID IN (623, 628) AND bLevel = 24 AND bNextLevel = 28 AND bMaxLevel = 3) <> 2
BEGIN
	RAISERROR('Refused: skills 623/628 are not in the expected original state (bLevel 24, bNextLevel 28, bMaxLevel 3).', 16, 1);
	RETURN;
END

BEGIN TRANSACTION;

UPDATE dbo.TSKILLCHART SET bNextLevel = 22 WHERE wID IN (623, 628) AND bNextLevel = 28;

IF @@ROWCOUNT <> 2
BEGIN
	ROLLBACK TRANSACTION;
	RAISERROR('Rolled back: expected exactly 2 rows to change.', 16, 1);
	RETURN;
END

COMMIT TRANSACTION;

SELECT wID, szName, bLevel, bNextLevel, bMaxLevel FROM dbo.TSKILLCHART WHERE wID IN (623, 628);
