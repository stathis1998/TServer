-- Remove the free, unconditioned link from the Mission 4 camp Region Portals
-- (9916 Valorian, 9917 Derion) to the Mission 8 entrance (9707).
-- Pre-existing in the original DB pack; it bypassed Mission 8's level 80-90 and item 29013 rules.
SET XACT_ABORT ON;
BEGIN TRAN;
DELETE TDESTINATIONCHART WHERE wPortalID IN (9916, 9917) AND wDestID = 9707;
IF @@ROWCOUNT <> 2 BEGIN RAISERROR('Expected to delete 2 rows', 16, 1); ROLLBACK; RETURN; END
COMMIT;
