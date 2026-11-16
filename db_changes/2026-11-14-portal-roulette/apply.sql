-- Apply: the portal's roulette.
--   TPORTALROULETTESPIN   every spin: who bet, where the ball landed, what was staked and paid, and the wallet afterwards
--   TPORTALSETTING        the switch that opens the table ('roulette_live') and the three bet limits; the table is shared with achievements and the wheel
-- The bets and the wallet change are made by the portal itself in one transaction, so there is no stored procedure.
-- Source of truth: 4story-portal apps/api/sql/, one file per object.
-- Safe to run twice. No restart needed. The setting rows are only added where missing, so a GM's changes survive a second run.
-- Nobody can bet until the switch is turned on from the portal's GM page.
-- Starting limits, in whole Silver: smallest bet 10, largest on one number 1000 (1 Gold), largest on any other spot 10000 (10 Gold).
USE TGAME_GSP;
GO
-- Portal settings that have to be the same for every portal process using this game database.
-- 'achievements_live' = 'true' lets TPortalAchievementAward award. Kept through the season reset.
IF OBJECT_ID('dbo.TPORTALSETTING', 'U') IS NULL
	CREATE TABLE dbo.TPORTALSETTING (
		szKey			VARCHAR(40)		NOT NULL CONSTRAINT PK_TPORTALSETTING PRIMARY KEY,
		szValue			VARCHAR(200)	NOT NULL,
		szUpdatedBy		VARCHAR(50)		NOT NULL,
		dUpdated		DATETIME		NOT NULL CONSTRAINT DF_TPORTALSETTING_dUpdated DEFAULT GETDATE()
	);
GO
-- Every spin of the portal's roulette, written by the bet batch in the same transaction that takes the
-- stake and pays the win from the character's wallet, so money never moves without its row.
--   Money is in Bronze (1 Silver = 1000 Bronze); szBets keeps what was staked where, in Silver.
--   The character's name is copied here, so a rename or delete later does not change history.
IF OBJECT_ID('dbo.TPORTALROULETTESPIN', 'U') IS NULL
BEGIN
	CREATE TABLE dbo.TPORTALROULETTESPIN (
		dwID			INT IDENTITY(1,1)	NOT NULL CONSTRAINT PK_TPORTALROULETTESPIN PRIMARY KEY,
		dwUserID		INT				NOT NULL,	-- the account
		dwCharID		INT				NOT NULL,	-- whose wallet paid and was paid
		szCharName		VARCHAR(50)		NOT NULL,
		dSpun			DATETIME		NOT NULL CONSTRAINT DF_TPORTALROULETTESPIN_dSpun DEFAULT GETDATE(),
		bPocket			TINYINT			NOT NULL,	-- 0 to 36
		llStake			BIGINT			NOT NULL,	-- Bronze
		llPayout		BIGINT			NOT NULL,	-- Bronze, the stake on winning bets included
		llWallet		BIGINT			NOT NULL,	-- Bronze, the wallet afterwards
		szBets			VARCHAR(1000)	NOT NULL,	-- 'n17:100,red:500', amounts in Silver
		bNumberHit		TINYINT			NOT NULL	-- 1 when a number bet won
	);
	CREATE INDEX IX_TPORTALROULETTESPIN_USER ON dbo.TPORTALROULETTESPIN (dwUserID, dwID);
END

GO
-- The switch starts off. Each key is added only if it is missing.
IF NOT EXISTS (SELECT 1 FROM dbo.TPORTALSETTING WHERE szKey = 'roulette_live')
	INSERT INTO dbo.TPORTALSETTING (szKey, szValue, szUpdatedBy) VALUES ('roulette_live', 'false', 'apply.sql');
IF NOT EXISTS (SELECT 1 FROM dbo.TPORTALSETTING WHERE szKey = 'roulette_min_bet')
	INSERT INTO dbo.TPORTALSETTING (szKey, szValue, szUpdatedBy) VALUES ('roulette_min_bet', '10', 'apply.sql');
IF NOT EXISTS (SELECT 1 FROM dbo.TPORTALSETTING WHERE szKey = 'roulette_max_number')
	INSERT INTO dbo.TPORTALSETTING (szKey, szValue, szUpdatedBy) VALUES ('roulette_max_number', '1000', 'apply.sql');
IF NOT EXISTS (SELECT 1 FROM dbo.TPORTALSETTING WHERE szKey = 'roulette_max_outside')
	INSERT INTO dbo.TPORTALSETTING (szKey, szValue, szUpdatedBy) VALUES ('roulette_max_outside', '10000', 'apply.sql');
GO
