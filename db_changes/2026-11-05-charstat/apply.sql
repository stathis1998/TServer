-- Adds per-character combat statistics: tables TCHARSTATTABLE and TCHARMONKILLTABLE, and their procedures.
-- Purely additive: changes no existing table or procedure, and needs no server restart by itself.
-- Apply BEFORE starting a TMapSvr built from the feature/char-combat-stats branch.
--   sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -i apply.sql
-- Runs as one transaction: any error stops the script and leaves the database unchanged.
:on error exit
SET NOCOUNT ON;
IF DB_NAME() NOT IN ('TGAME_GSP', 'TGAME_GSP_TEST') THROW 50000, 'Run this in TGAME_GSP or TGAME_GSP_TEST, using sqlcmd -d.', 1;
IF OBJECT_ID('dbo.TCHARSTATTABLE') IS NOT NULL OR OBJECT_ID('dbo.TCHARMONKILLTABLE') IS NOT NULL
	OR OBJECT_ID('dbo.TGetCharStat') IS NOT NULL OR OBJECT_ID('dbo.TGetCharMonKill') IS NOT NULL
	OR OBJECT_ID('dbo.TSaveCharStat') IS NOT NULL OR OBJECT_ID('dbo.TSaveCharMonKill') IS NOT NULL
	THROW 50000, 'Combat statistics objects already exist in this database. Run undo.sql first.', 1;
BEGIN TRANSACTION;
GO

CREATE TABLE dbo.TCHARSTATTABLE (
	dwCharID     int      NOT NULL,
	dwTotalKills int      NOT NULL CONSTRAINT DF_TCHARSTATTABLE_dwTotalKills DEFAULT 0,
	nDamageDealt bigint   NOT NULL CONSTRAINT DF_TCHARSTATTABLE_nDamageDealt DEFAULT 0,
	nDamageTaken bigint   NOT NULL CONSTRAINT DF_TCHARSTATTABLE_nDamageTaken DEFAULT 0,
	nHealDone    bigint   NOT NULL CONSTRAINT DF_TCHARSTATTABLE_nHealDone    DEFAULT 0,
	dUpdate      datetime NOT NULL CONSTRAINT DF_TCHARSTATTABLE_dUpdate      DEFAULT GETDATE(),
	CONSTRAINT PK_TCHARSTATTABLE PRIMARY KEY CLUSTERED (dwCharID)
);
GO

-- wMonID is TMONSTERCHART.wID, the monster template ID, which is also smallint.
CREATE TABLE dbo.TCHARMONKILLTABLE (
	dwCharID int      NOT NULL,
	wMonID   smallint NOT NULL,
	dwKills  int      NOT NULL CONSTRAINT DF_TCHARMONKILLTABLE_dwKills DEFAULT 0,
	nDamage  bigint   NOT NULL CONSTRAINT DF_TCHARMONKILLTABLE_nDamage DEFAULT 0,
	dUpdate  datetime NOT NULL CONSTRAINT DF_TCHARMONKILLTABLE_dUpdate DEFAULT GETDATE(),
	CONSTRAINT PK_TCHARMONKILLTABLE PRIMARY KEY CLUSTERED (dwCharID, wMonID)
);
GO

CREATE PROCEDURE dbo.TGetCharStat
	@dwCharID     int,
	@dwTotalKills int OUTPUT,
	@nDamageDealt bigint OUTPUT,
	@nDamageTaken bigint OUTPUT,
	@nHealDone    bigint OUTPUT
AS
SET NOCOUNT ON;
SET @dwTotalKills = 0;
SET @nDamageDealt = 0;
SET @nDamageTaken = 0;
SET @nHealDone = 0;
SELECT @dwTotalKills = dwTotalKills, @nDamageDealt = nDamageDealt, @nDamageTaken = nDamageTaken, @nHealDone = nHealDone
FROM dbo.TCHARSTATTABLE WHERE dwCharID = @dwCharID;
GO

-- For checking a character by hand. The server reads the table with its own SELECT.
CREATE PROCEDURE dbo.TGetCharMonKill
	@dwCharID int
AS
SET NOCOUNT ON;
SELECT wMonID, dwKills, nDamage, dUpdate FROM dbo.TCHARMONKILLTABLE WHERE dwCharID = @dwCharID ORDER BY dwKills DESC, wMonID;
GO

-- The save procedures only ever raise a value. A late or repeated write, such as the old server's
-- flush arriving after a channel change, cannot lower a counter.
CREATE PROCEDURE dbo.TSaveCharStat
	@dwCharID     int,
	@dwTotalKills int,
	@nDamageDealt bigint,
	@nDamageTaken bigint,
	@nHealDone    bigint
AS
SET NOCOUNT ON;
UPDATE dbo.TCHARSTATTABLE SET
	dwTotalKills = CASE WHEN @dwTotalKills > dwTotalKills THEN @dwTotalKills ELSE dwTotalKills END,
	nDamageDealt = CASE WHEN @nDamageDealt > nDamageDealt THEN @nDamageDealt ELSE nDamageDealt END,
	nDamageTaken = CASE WHEN @nDamageTaken > nDamageTaken THEN @nDamageTaken ELSE nDamageTaken END,
	nHealDone    = CASE WHEN @nHealDone    > nHealDone    THEN @nHealDone    ELSE nHealDone    END,
	dUpdate      = GETDATE()
WHERE dwCharID = @dwCharID;
IF @@ROWCOUNT = 0
	INSERT INTO dbo.TCHARSTATTABLE (dwCharID, dwTotalKills, nDamageDealt, nDamageTaken, nHealDone)
	VALUES (@dwCharID, @dwTotalKills, @nDamageDealt, @nDamageTaken, @nHealDone);
GO

CREATE PROCEDURE dbo.TSaveCharMonKill
	@dwCharID int,
	@wMonID   smallint,
	@dwKills  int,
	@nDamage  bigint
AS
SET NOCOUNT ON;
UPDATE dbo.TCHARMONKILLTABLE SET
	dwKills = CASE WHEN @dwKills > dwKills THEN @dwKills ELSE dwKills END,
	nDamage = CASE WHEN @nDamage > nDamage THEN @nDamage ELSE nDamage END,
	dUpdate = GETDATE()
WHERE dwCharID = @dwCharID AND wMonID = @wMonID;
IF @@ROWCOUNT = 0
	INSERT INTO dbo.TCHARMONKILLTABLE (dwCharID, wMonID, dwKills, nDamage)
	VALUES (@dwCharID, @wMonID, @dwKills, @nDamage);
GO

COMMIT;
PRINT 'Combat statistics objects created in ' + DB_NAME();
GO
