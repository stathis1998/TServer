-- Undo apply.sql: chakras, auras and wings back to Valorian only (bItemCountry 0) and price 0,
-- and Maytu (26011) stops selling the 5 wings and Bats Cloud that apply.sql added (not Blue/Sacred Aura, added separately). Restart TMapSvr afterwards.
SET XACT_ABORT ON;
SET NOCOUNT ON;
BEGIN TRAN;
UPDATE TITEMCHART SET bItemCountry = 0, fPrice = 0
WHERE bType = 17 AND bKind = 250;
DELETE TNPCITEMCHART WHERE wNpcID = 26011 AND dwItemID IN (25004,25009,25010,25013,25014);
COMMIT;
