-- Apply: the portal's Wheel of Fortune.
--   TPORTALWHEELPRIZE       the prizes on the wheel, edited by a GM on the portal; ten starting prizes are inserted
--   TPORTALWHEELSPINTABLE   every spin: one per account and day, with what it paid
--   TPORTALSETTING          the switch that lets players spin ('wheel_live'); the table is shared with achievements
--   TPortalWheelSpin        checks a spin is allowed, mails the prize and records the spin, in one transaction
-- Source of truth: 4story-portal apps/api/sql/, one file per object.
-- Needs TPortalGmPost and TPORTALPOSTNOTIFY (2026-11-10 and 2026-11-11). Safe to run twice. No restart needed.
-- Nobody can spin until the switch is turned on from the portal's GM page.
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
-- The prizes on the portal's Wheel of Fortune, edited by a GM on /gm/wheel.
--   A prize's chance is its wWeight out of the total wWeight of the enabled prizes.
--   The reward has the shape of one letter: money and up to 3 items. A prize with no money and no
--   items is a "no luck" segment and sends no letter.
-- TPortalWheelSpin reads the prize inside the spin's own transaction.
-- The ten starting prizes are inserted once, when the table is created. Their names are kept to
-- 22 characters, which is what fits along a segment of the wheel.
IF OBJECT_ID('dbo.TPORTALWHEELPRIZE', 'U') IS NULL
BEGIN
	CREATE TABLE dbo.TPORTALWHEELPRIZE (
		dwID			INT IDENTITY(1,1)	NOT NULL CONSTRAINT PK_TPORTALWHEELPRIZE PRIMARY KEY,
		szName			VARCHAR(40)		NOT NULL,	-- on the wheel, in the letter and in the announcement
		wWeight			SMALLINT		NOT NULL,
		wOrder			SMALLINT		NOT NULL,	-- position on the wheel, lowest first
		bEnabled		TINYINT			NOT NULL CONSTRAINT DF_TPORTALWHEELPRIZE_bEnabled DEFAULT 1,
		bAnnounce		TINYINT			NOT NULL CONSTRAINT DF_TPORTALWHEELPRIZE_bAnnounce DEFAULT 0,	-- 1: a win is said in world chat
		dwGold			INT				NOT NULL,
		dwSilver		INT				NOT NULL,
		dwCooper		INT				NOT NULL,
		wItemID1		SMALLINT		NOT NULL,	-- 0 for an empty slot
		bCount1			TINYINT			NOT NULL,
		wItemID2		SMALLINT		NOT NULL,
		bCount2			TINYINT			NOT NULL,
		wItemID3		SMALLINT		NOT NULL,
		bCount3			TINYINT			NOT NULL,
		szUpdatedBy		VARCHAR(50)		NOT NULL,
		dUpdated		DATETIME		NOT NULL CONSTRAINT DF_TPORTALWHEELPRIZE_dUpdated DEFAULT GETDATE()
	);

	-- The weights add up to 100. Small and big prizes alternate around the wheel.
	INSERT INTO dbo.TPORTALWHEELPRIZE
		(szName, wWeight, wOrder, bAnnounce, dwGold, dwSilver, dwCooper, wItemID1, bCount1, wItemID2, bCount2, wItemID3, bCount3, szUpdatedBy)
	VALUES
		('100 Luna',                        22,  10, 0,  0, 100, 0,    0,  0, 0, 0, 0, 0, 'portal'),
		('Experience Potion',                4,  20, 0,  0,   0, 0, 7714,  1, 0, 0, 0, 0, 'portal'),
		('5 Apprentice Formulas',           16,  30, 0,  0,   0, 0, 8501,  5, 0, 0, 0, 0, 'portal'),
		('1 Rune',                           6,  40, 0,  1,   0, 0,    0,  0, 0, 0, 0, 0, 'portal'),
		('300 Luna',                        15,  50, 0,  0, 300, 0,    0,  0, 0, 0, 0, 0, 'portal'),
		('Master''s Formula',               10,  60, 0,  0,   0, 0, 7602,  1, 0, 0, 0, 0, 'portal'),
		('10 Rune',                          1,  70, 1, 10,   0, 0,    0,  0, 0, 0, 0, 0, 'portal'),
		('2 Serendipity 100%',              10,  80, 0,  0,   0, 0, 7609,  2, 0, 0, 0, 0, 'portal'),
		('Survival Tincture',                8,  90, 0,  0,   0, 0, 7654,  1, 0, 0, 0, 0, 'portal'),
		('10 Healing Potions',               8, 100, 0,  0,   0, 0, 7600, 10, 0, 0, 0, 0, 'portal');
END
GO
-- Every spin of the portal's Wheel of Fortune, written by TPortalWheelSpin together with the letter
-- that carries the prize, so a spin is never paid twice and never recorded without its letter.
--   One row per account and day: the unique key is what limits an account to one spin a day.
--   The prize's name and what it paid are copied here, so editing or deleting a prize later does
--   not change history.
IF OBJECT_ID('dbo.TPORTALWHEELSPINTABLE', 'U') IS NULL
	CREATE TABLE dbo.TPORTALWHEELSPINTABLE (
		dwID			INT IDENTITY(1,1)	NOT NULL CONSTRAINT PK_TPORTALWHEELSPINTABLE PRIMARY KEY,
		dwUserID		INT				NOT NULL,
		dDay			DATE			NOT NULL,	-- the server's day this spin used up
		dwCharID		INT				NOT NULL,	-- who got the letter
		szCharName		VARCHAR(50)		NOT NULL,
		dSpun			DATETIME		NOT NULL CONSTRAINT DF_TPORTALWHEELSPINTABLE_dSpun DEFAULT GETDATE(),
		dwPrizeID		INT				NOT NULL,
		szPrize			VARCHAR(40)		NOT NULL,
		dwPostID		INT				NOT NULL,	-- 0 when the prize was empty and no letter was sent
		dwGold			INT				NOT NULL,	-- what the letter carried
		dwSilver		INT				NOT NULL,
		dwCooper		INT				NOT NULL,
		szItems			VARCHAR(60)		NOT NULL,	-- '7602x5,7654x1', empty for none
		szText			VARCHAR(200)	NOT NULL,	-- the announcement line
		bAnnounce		TINYINT			NOT NULL,	-- 1 when the prize was one that is announced
		bAnnounced		TINYINT			NOT NULL,	-- 0 waiting, 1 sent, 2 skipped or never due
		CONSTRAINT UQ_TPORTALWHEELSPINTABLE UNIQUE (dwUserID, dDay)
	);
GO
-- One spin of the portal's Wheel of Fortune: checks it is allowed, sends the prize to the character
-- as a letter through TPortalGmPost and records the spin, in one transaction.
-- The portal picks the prize; this procedure reads what it pays, so the reward is the one saved at
-- this moment.
--   {prize} in @pTitle, @pMessage and @szText becomes the prize's name, {name} the character's.
-- Returns the new dwID, or
--    0 when the account has already spun today (the database's own date),
--   -1 when the character is not this account's, is deleted or is under @bMinLevel,
--   -2 while the wheel is switched off (TPORTALSETTING 'wheel_live'),
--   -3 when the prize does not exist or is disabled.
-- Nothing is written in those four cases, so they end with a COMMIT: that keeps a caller's own
-- transaction intact.
CREATE OR ALTER PROCEDURE dbo.TPortalWheelSpin
	@dwUserID		INT,
	@dwCharID		INT,
	@dwPrizeID		INT,
	@bMinLevel		TINYINT,
	@pSender		VARCHAR(50),
	@pTitle			VARCHAR(200),
	@pMessage		VARCHAR(1800),
	@szText			VARCHAR(160)
AS
BEGIN
	SET NOCOUNT ON;
	SET XACT_ABORT ON;

	DECLARE @dDay DATE = CAST(GETDATE() AS DATE), @dwPostID INT = 0, @dwID INT,
		@szCharName VARCHAR(50), @szPrize VARCHAR(40), @bAnnounce TINYINT, @szItems VARCHAR(60) = '',
		@szTitle VARCHAR(240), @szMessage VARCHAR(2000), @szLine VARCHAR(200),
		@dwGold INT, @dwSilver INT, @dwCooper INT,
		@wItemID1 SMALLINT, @bCount1 TINYINT,
		@wItemID2 SMALLINT, @bCount2 TINYINT,
		@wItemID3 SMALLINT, @bCount3 TINYINT;

	BEGIN TRAN;

	IF NOT EXISTS (SELECT 1 FROM TPORTALSETTING WHERE szKey = 'wheel_live' AND szValue = 'true')
	BEGIN
		COMMIT;
		RETURN -2;
	END

	-- Held to the end of the transaction, so two spins by one account can't both get past here.
	IF EXISTS (SELECT 1 FROM TPORTALWHEELSPINTABLE WITH (UPDLOCK, HOLDLOCK) WHERE dwUserID = @dwUserID AND dDay = @dDay)
	BEGIN
		COMMIT;
		RETURN 0;
	END

	SELECT @szCharName = szName FROM TCHARTABLE
	WHERE dwCharID = @dwCharID AND dwUserID = @dwUserID AND bDelete = 0 AND bLevel >= @bMinLevel;
	IF @szCharName IS NULL
	BEGIN
		COMMIT;
		RETURN -1;
	END

	SELECT @szPrize = szName, @bAnnounce = bAnnounce,
		@dwGold = dwGold, @dwSilver = dwSilver, @dwCooper = dwCooper,
		@wItemID1 = wItemID1, @bCount1 = bCount1,
		@wItemID2 = wItemID2, @bCount2 = bCount2,
		@wItemID3 = wItemID3, @bCount3 = bCount3
	FROM TPORTALWHEELPRIZE
	WHERE dwID = @dwPrizeID AND bEnabled = 1;
	IF @szPrize IS NULL
	BEGIN
		COMMIT;
		RETURN -3;
	END

	SET @szTitle = REPLACE(REPLACE(@pTitle, '{prize}', @szPrize), '{name}', @szCharName);
	SET @szMessage = REPLACE(REPLACE(@pMessage, '{prize}', @szPrize), '{name}', @szCharName);
	SET @szLine = REPLACE(REPLACE(@szText, '{prize}', @szPrize), '{name}', @szCharName);

	-- An empty prize is a "no luck" segment: it uses up the spin and sends nothing.
	IF @dwGold + @dwSilver + @dwCooper > 0 OR @wItemID1 <> 0 OR @wItemID2 <> 0 OR @wItemID3 <> 0
	BEGIN
		EXEC @dwPostID = dbo.TPortalGmPost
			@pCharID = @dwCharID, @pSender = @pSender, @pTitle = @szTitle, @pMessage = @szMessage,
			@dwGold = @dwGold, @dwSilver = @dwSilver, @dwCooper = @dwCooper,
			@wItemID1 = @wItemID1, @bCount1 = @bCount1,
			@wItemID2 = @wItemID2, @bCount2 = @bCount2,
			@wItemID3 = @wItemID3, @bCount3 = @bCount3;

		-- TPortalGmPost returns 0 before it writes anything.
		IF @dwPostID = 0
		BEGIN
			COMMIT;
			RETURN -1;
		END

		IF @wItemID1 <> 0 SET @szItems = CAST(@wItemID1 AS VARCHAR(6)) + 'x' + CAST(@bCount1 AS VARCHAR(3));
		IF @wItemID2 <> 0 SET @szItems = @szItems + CASE WHEN @szItems = '' THEN '' ELSE ',' END + CAST(@wItemID2 AS VARCHAR(6)) + 'x' + CAST(@bCount2 AS VARCHAR(3));
		IF @wItemID3 <> 0 SET @szItems = @szItems + CASE WHEN @szItems = '' THEN '' ELSE ',' END + CAST(@wItemID3 AS VARCHAR(6)) + 'x' + CAST(@bCount3 AS VARCHAR(3));
	END

	INSERT INTO TPORTALWHEELSPINTABLE
		(dwUserID, dDay, dwCharID, szCharName, dwPrizeID, szPrize, dwPostID, dwGold, dwSilver, dwCooper, szItems, szText, bAnnounce, bAnnounced)
	VALUES
		(@dwUserID, @dDay, @dwCharID, @szCharName, @dwPrizeID, @szPrize, @dwPostID, @dwGold, @dwSilver, @dwCooper, @szItems, @szLine,
			@bAnnounce, CASE WHEN @bAnnounce = 1 THEN 0 ELSE 2 END);

	SET @dwID = SCOPE_IDENTITY();

	COMMIT;
	RETURN @dwID;
END
GO
