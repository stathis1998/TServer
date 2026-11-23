-- Steadfast saddle: item 31307 (an unused permanent Saddle of Swiftness) becomes the Steadfast Saddle.
-- A TMapSvr built from feature/steadfast-saddle keeps its rider mounted when monsters hit them.
-- Only the server's name changes here; the client's TItem.tcd still says Saddle of Swiftness until it is edited.
-- Safe to run twice. Undo with undo.sql.
SET XACT_ABORT ON;
BEGIN TRAN;
UPDATE dbo.TITEMCHART SET szNAME = 'Steadfast Saddle'
WHERE wItemID = 31307 AND bType = 21 AND bKind = 98 AND szNAME IN ('Saddle of Swiftness', 'Steadfast Saddle');
IF @@ROWCOUNT <> 1
BEGIN
    RAISERROR('Item 31307 is not the saddle this script expects, nothing changed', 16, 1);
    ROLLBACK; RETURN;
END
COMMIT;
SELECT wItemID, bType, bKind, szNAME, wUseValue, wUseTime, bUseType FROM dbo.TITEMCHART WHERE wItemID = 31307;
