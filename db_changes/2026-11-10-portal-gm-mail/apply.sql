-- Apply: adds dbo.TPortalGmPost (portal GM mail). Source of truth: 4story-portal apps/api/sql/TPortalGmPost.sql
USE TGAME_GSP;
GO
-- GM mail from the portal: one Operator letter (bType 6) with optional money and up to 3 items.
-- Mirrors OPTool_GamePost / TEventItemGiveByPost. Validation happens in the portal API; this procedure
-- writes no audit row (the API does, through OPTool_ManagerLog).
-- Returns the new dwPostID, or 0 when the character does not exist or is deleted.
CREATE OR ALTER PROCEDURE dbo.TPortalGmPost
	@pCharID	INT,
	@pSender	VARCHAR(50),
	@pTitle		VARCHAR(240),
	@pMessage	VARCHAR(2000),
	@dwGold		INT,
	@dwSilver	INT,
	@dwCooper	INT,
	@wItemID1	SMALLINT,
	@bCount1	TINYINT,
	@wItemID2	SMALLINT,
	@bCount2	TINYINT,
	@wItemID3	SMALLINT,
	@bCount3	TINYINT
AS
BEGIN
	SET NOCOUNT ON;
	SET XACT_ABORT ON;

	DECLARE @szRecv VARCHAR(50), @dwPostID INT, @dlID BIGINT, @dwDuraMax INT,
		@slot INT = 1, @wItem SMALLINT, @bCount TINYINT;

	SELECT @szRecv = szName FROM TCHARTABLE WHERE dwCharID = @pCharID AND bDelete = 0;
	IF @szRecv IS NULL RETURN 0;

	-- The game stores title and message behind an 8-character hex length prefix.
	DECLARE @szTitle VARCHAR(256) =
		RIGHT(master.dbo.fn_sqlvarbasetostr(CAST(DATALENGTH(@pTitle) AS BINARY(4))), 8) + @pTitle;
	DECLARE @szMessage VARCHAR(2048) =
		RIGHT(master.dbo.fn_sqlvarbasetostr(CAST(DATALENGTH(@pMessage) AS BINARY(4))), 8) + @pMessage;

	BEGIN TRAN;

	INSERT INTO TPOSTTABLE
		(dwCharID, szSender, dwSendID, szRecvName, szTitle, szMessage, bType, bRead, dwGold, dwSilver, dwCooper, timeRecv)
	VALUES
		(@pCharID, @pSender, 0, @szRecv, @szTitle, @szMessage, 6, 0, @dwGold, @dwSilver, @dwCooper, GETDATE());

	SET @dwPostID = SCOPE_IDENTITY();

	WHILE @slot <= 3
	BEGIN
		SELECT
			@wItem = CASE @slot WHEN 1 THEN @wItemID1 WHEN 2 THEN @wItemID2 ELSE @wItemID3 END,
			@bCount = CASE @slot WHEN 1 THEN @bCount1 WHEN 2 THEN @bCount2 ELSE @bCount3 END;

		IF @wItem <> 0
		BEGIN
			SET @dwDuraMax = 0;
			SELECT @dwDuraMax = dwDuraMax FROM TITEMCHART WHERE wItemID = @wItem;

			EXEC TGenerateDBItemID @dlID OUTPUT;

			INSERT INTO TITEMTABLE
				(dlID, bStorageType, dwStorageID, bOwnerType, dwOwnerID, bItemID, wItemID, bLevel, bCount, bGLevel,
				 dwDuraMax, dwDuraCur, bRefineCur, dEndTime, bGradeEffect,
				 bMagic1, bMagic2, bMagic3, bMagic4, bMagic5, bMagic6,
				 wValue1, wValue2, wValue3, wValue4, wValue5, wValue6,
				 dwTime1, dwTime2, dwTime3, dwTime4, dwTime5, dwTime6, bGem, wMoggItemID)
			VALUES
				(@dlID, 2, @dwPostID, 0, @pCharID, 0, @wItem, 0, @bCount, 0,
				 @dwDuraMax, @dwDuraMax, 0, '1900-01-01', 0,
				 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0);
		END

		SET @slot += 1;
	END

	COMMIT;
	RETURN @dwPostID;
END
GO
