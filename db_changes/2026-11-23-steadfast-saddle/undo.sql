-- Undo 2026-11-23-steadfast-saddle: item 31307 goes back to its old name.
SET XACT_ABORT ON;
UPDATE dbo.TITEMCHART SET szNAME = 'Saddle of Swiftness' WHERE wItemID = 31307 AND szNAME = 'Steadfast Saddle';
SELECT wItemID, szNAME FROM dbo.TITEMCHART WHERE wItemID = 31307;
