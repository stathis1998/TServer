-- Apply: tiered achievements on the portal.
--   TPORTALACHIEVEMENTTABLE   levels awarded this season (also the portal's news feed): per character
--                             and rebirth for character achievements, per account for account ones
--   TPORTALACHIEVEMENTREWARD  what each character achievement level pays, set by a GM on the portal
--   TPORTALSETTING            the switch that lets levels be awarded ('achievements_live')
--   TPortalAchievementAward   records an award and, for a character, sends the reward letter, in one transaction
-- Source of truth: 4story-portal apps/api/sql/, one file per object.
-- Needs TPortalGmPost and TPORTALPOSTNOTIFY (2026-11-10 and 2026-11-11). Safe to run twice. No restart needed.
-- Nothing is awarded until the switch is turned on from the portal's GM page.
USE TGAME_GSP;
GO
-- Achievement levels the portal has awarded, written by TPortalAchievementAward together with the
-- letter that carries the reward, so a level is never paid twice and never recorded without its
-- letter. The portal's "Latest news" reads szText from here.
--   A character achievement is awarded to one character (dwCharID) in one of its lives (bRebirth,
--   0 until rebirth exists). Raising a character's bRebirth makes every level earnable again and
--   keeps the earlier rows as history.
--   An account achievement has dwCharID 0 and sends no letter (dwPostID 0).
-- Emptied by the season reset, like the characters it points at.
IF OBJECT_ID('dbo.TPORTALACHIEVEMENTTABLE', 'U') IS NULL
	CREATE TABLE dbo.TPORTALACHIEVEMENTTABLE (
		dwID			INT IDENTITY(1,1)	NOT NULL CONSTRAINT PK_TPORTALACHIEVEMENTTABLE PRIMARY KEY,
		dwUserID		INT				NOT NULL,
		dwCharID		INT				NOT NULL,	-- 0 for an account achievement
		bRebirth		TINYINT			NOT NULL,
		szAchievement	VARCHAR(40)		NOT NULL,
		bLevel			TINYINT			NOT NULL,
		dwPostID		INT				NOT NULL,	-- 0 when no letter was sent
		dEarned			DATETIME		NOT NULL CONSTRAINT DF_TPORTALACHIEVEMENTTABLE_dEarned DEFAULT GETDATE(),
		dwGold			INT				NOT NULL,	-- what the letter carried
		dwSilver		INT				NOT NULL,
		dwCooper		INT				NOT NULL,
		szItems			VARCHAR(60)		NOT NULL,	-- '7602x5,7654x1', empty for none
		szText			VARCHAR(200)	NOT NULL,	-- the announcement line
		bAnnounced		TINYINT			NOT NULL CONSTRAINT DF_TPORTALACHIEVEMENTTABLE_bAnnounced DEFAULT 0,	-- 0 waiting, 1 sent, 2 skipped
		CONSTRAINT UQ_TPORTALACHIEVEMENTTABLE UNIQUE (dwUserID, dwCharID, szAchievement, bLevel, bRebirth)
	);
GO
-- What each character achievement level pays, set by a GM on the portal. A level with no row pays
-- nothing, and account achievements never pay.
-- TPortalAchievementAward reads it inside the award's own transaction. Kept through the season reset.
IF OBJECT_ID('dbo.TPORTALACHIEVEMENTREWARD', 'U') IS NULL
	CREATE TABLE dbo.TPORTALACHIEVEMENTREWARD (
		szAchievement	VARCHAR(40)	NOT NULL,
		bLevel			TINYINT		NOT NULL,
		dwGold			INT			NOT NULL,
		dwSilver		INT			NOT NULL,
		dwCooper		INT			NOT NULL,
		wItemID1		SMALLINT	NOT NULL,	-- 0 for an empty slot
		bCount1			TINYINT		NOT NULL,
		wItemID2		SMALLINT	NOT NULL,
		bCount2			TINYINT		NOT NULL,
		wItemID3		SMALLINT	NOT NULL,
		bCount3			TINYINT		NOT NULL,
		szUpdatedBy		VARCHAR(50)	NOT NULL,
		dUpdated		DATETIME	NOT NULL CONSTRAINT DF_TPORTALACHIEVEMENTREWARD_dUpdated DEFAULT GETDATE(),
		CONSTRAINT PK_TPORTALACHIEVEMENTREWARD PRIMARY KEY (szAchievement, bLevel)
	);
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
-- Awards one achievement level and records it, in one transaction.
--   @dwCharID <> 0: a character achievement. That character gets a letter through TPortalGmPost,
--                   with the reward saved for the level in TPORTALACHIEVEMENTREWARD.
--   @dwCharID = 0:  an account achievement. No letter and no reward: it is only recorded.
-- Returns the new dwID, or
--    0 when the level was already awarded (to this character in this life, or to this account),
--   -1 when the character does not exist or is deleted,
--   -2 while rewards are switched off (TPORTALSETTING 'achievements_live').
-- Nothing is written in those three cases, so they end with a COMMIT: that keeps a caller's own
-- transaction intact.
CREATE OR ALTER PROCEDURE dbo.TPortalAchievementAward
	@dwUserID		INT,
	@dwCharID		INT,
	@bRebirth		TINYINT,
	@szAchievement	VARCHAR(40),
	@bLevel			TINYINT,
	@pSender		VARCHAR(50),
	@pTitle			VARCHAR(240),
	@pMessage		VARCHAR(1800),
	@pRewardNote	VARCHAR(200),	-- added to the message when the level pays something
	@szText			VARCHAR(200)
AS
BEGIN
	SET NOCOUNT ON;
	SET XACT_ABORT ON;

	DECLARE @dwPostID INT = 0, @dwID INT, @szItems VARCHAR(60) = '', @szMessage VARCHAR(2000) = @pMessage,
		@dwGold INT = 0, @dwSilver INT = 0, @dwCooper INT = 0,
		@wItemID1 SMALLINT = 0, @bCount1 TINYINT = 0,
		@wItemID2 SMALLINT = 0, @bCount2 TINYINT = 0,
		@wItemID3 SMALLINT = 0, @bCount3 TINYINT = 0;

	BEGIN TRAN;

	IF NOT EXISTS (SELECT 1 FROM TPORTALSETTING WHERE szKey = 'achievements_live' AND szValue = 'true')
	BEGIN
		COMMIT;
		RETURN -2;
	END

	-- Held to the end of the transaction, so two calls for the same level can't both get past here.
	IF EXISTS (SELECT 1 FROM TPORTALACHIEVEMENTTABLE WITH (UPDLOCK, HOLDLOCK)
		WHERE dwUserID = @dwUserID AND dwCharID = @dwCharID AND szAchievement = @szAchievement
			AND bLevel = @bLevel AND bRebirth = @bRebirth)
	BEGIN
		COMMIT;
		RETURN 0;
	END

	IF @dwCharID <> 0
	BEGIN
		SELECT @dwGold = dwGold, @dwSilver = dwSilver, @dwCooper = dwCooper,
			@wItemID1 = wItemID1, @bCount1 = bCount1,
			@wItemID2 = wItemID2, @bCount2 = bCount2,
			@wItemID3 = wItemID3, @bCount3 = bCount3
		FROM TPORTALACHIEVEMENTREWARD
		WHERE szAchievement = @szAchievement AND bLevel = @bLevel;

		IF @dwGold + @dwSilver + @dwCooper > 0 OR @wItemID1 <> 0 OR @wItemID2 <> 0 OR @wItemID3 <> 0
			SET @szMessage = @pMessage + @pRewardNote;

		EXEC @dwPostID = dbo.TPortalGmPost
			@pCharID = @dwCharID, @pSender = @pSender, @pTitle = @pTitle, @pMessage = @szMessage,
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

	INSERT INTO TPORTALACHIEVEMENTTABLE
		(dwUserID, dwCharID, bRebirth, szAchievement, bLevel, dwPostID, dwGold, dwSilver, dwCooper, szItems, szText)
	VALUES
		(@dwUserID, @dwCharID, @bRebirth, @szAchievement, @bLevel, @dwPostID, @dwGold, @dwSilver, @dwCooper, @szItems, @szText);

	SET @dwID = SCOPE_IDENTITY();

	COMMIT;
	RETURN @dwID;
END
GO
