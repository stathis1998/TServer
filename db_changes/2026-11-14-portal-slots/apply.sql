-- Apply: the portal's slot machines.
--   TPORTALSLOTSPIN   every spin: who played, which machine, where the three reels stopped, what was staked and paid, and the wallet afterwards
--   TPORTALSETTING    already there from achievements, the wheel and roulette; created here only if missing
-- The bets and the wallet change are made by the portal itself in one transaction (the same step roulette uses), so there is no stored procedure.
-- Source of truth: 4story-portal apps/api/sql/, one file per object.
-- Safe to run twice. No restart needed.
-- No setting rows are added. With none, the slots floor is closed, every machine is open and the stakes are the starting ones,
-- so nobody can spin until a GM opens the floor from the portal's GM page. The keys are slots_live, slots_<machine>_live and slots_<machine>_stakes.
-- Starting stakes, in whole Silver: Tranus's Shelf 10, 25, 50; The Armoury 25, 50, 100; Dragon's Hoard 50, 100, 200.
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
-- Every spin of the portal's slot machines, written by the bet batch in the same transaction that takes the
-- stake and pays the win from the character's wallet, so money never moves without its row.
--   Money is in Bronze (1 Silver = 1000 Bronze).
--   The character's name is copied here, so a rename or delete later does not change history.
IF OBJECT_ID('dbo.TPORTALSLOTSPIN', 'U') IS NULL
BEGIN
	CREATE TABLE dbo.TPORTALSLOTSPIN (
		dwID			INT IDENTITY(1,1)	NOT NULL CONSTRAINT PK_TPORTALSLOTSPIN PRIMARY KEY,
		dwUserID		INT				NOT NULL,	-- the account
		dwCharID		INT				NOT NULL,	-- whose wallet paid and was paid
		szCharName		VARCHAR(50)		NOT NULL,
		dSpun			DATETIME		NOT NULL CONSTRAINT DF_TPORTALSLOTSPIN_dSpun DEFAULT GETDATE(),
		szMachine		VARCHAR(20)		NOT NULL,	-- 'shelf', 'armoury', 'hoard'
		bStop1			TINYINT			NOT NULL,	-- 0 to 31, where each reel stopped
		bStop2			TINYINT			NOT NULL,
		bStop3			TINYINT			NOT NULL,
		llStake			BIGINT			NOT NULL,	-- Bronze
		llPayout		BIGINT			NOT NULL,	-- Bronze, the stake included
		llWallet		BIGINT			NOT NULL,	-- Bronze, the wallet afterwards
		wMultiple		SMALLINT		NOT NULL	-- the prize as a multiple of the stake, 0 for none
	);
	CREATE INDEX IX_TPORTALSLOTSPIN_USER ON dbo.TPORTALSLOTSPIN (dwUserID, dwID);
END

GO
