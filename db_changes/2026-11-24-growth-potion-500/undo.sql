-- Undo 2026-11-24-growth-potion-500: item 30309 goes back to a 300% Growth Potion (1 hour). Restart TMapSvr afterwards.
SET XACT_ABORT ON;
UPDATE dbo.TITEMCHART SET wUseValue = 200, szNAME = '300% Growth Potion (1 hour)' WHERE wItemID = 30309 AND bKind = 56 AND wUseValue = 400;
SELECT wItemID, szNAME, wUseValue FROM dbo.TITEMCHART WHERE wItemID = 30309;
