-- Make every chakra, aura and wing (TITEMCHART bType 17, bKind 250: 18 items, 25004-25061) visible at Maytu (NPC 26011)
-- and give them a test price of 100 silver.
--   bItemCountry 0 -> 3: 0 is Valorian only, so the server left them out of every shop list for Derion characters.
--                        3 (TCONTRY_N) means every country. See OnCS_NPCITEMLIST / SendCS_NPCITEMLIST_ACK in Server\TMapSvr\CSSender.cpp.
--   fPrice 0 -> 100000: the shop price is TLEVELCHART.dwMoney at the item's bLevel (1, which is 1) times fPrice,
--                        so this is 100,000 copper = 100 silver. They still can't be sold back (bIsSell 5 has no ITEMTRADE_SELL bit).
--   Adds the 5 that no NPC sold to Maytu: Dark Wings, Ice Wings, White Wings, Bats Cloud, Blue Wings.
--   Blue Aura (25051) and Sacred Aura (25053) were already added to Maytu separately on 2026-11-10, so this leaves them alone.
-- Restart TMapSvr afterwards: it reads these charts at startup. No client change is needed.
--   sqlcmd -S "localhost\FOURSTORY" -E -C -d TGAME_GSP -i apply.sql
SET XACT_ABORT ON;
SET NOCOUNT ON;
BEGIN TRAN;

IF (SELECT COUNT(*) FROM TITEMCHART WHERE bType = 17 AND bKind = 250) <> 18
BEGIN RAISERROR('Expected 18 chakra/aura/wing items (bType 17, bKind 250)', 16, 1); ROLLBACK; RETURN; END

IF EXISTS (SELECT 1 FROM TITEMCHART WHERE bType = 17 AND bKind = 250 AND (bItemCountry <> 0 OR fPrice <> 0))
BEGIN RAISERROR('Some items no longer have bItemCountry 0 and fPrice 0; check them before running this', 16, 1); ROLLBACK; RETURN; END

IF EXISTS (SELECT 1 FROM TNPCITEMCHART WHERE wNpcID = 26011 AND dwItemID IN (25004,25009,25010,25013,25014))
BEGIN RAISERROR('Maytu already sells some of the items this script adds', 16, 1); ROLLBACK; RETURN; END

UPDATE TITEMCHART SET bItemCountry = 3, fPrice = 100000
WHERE bType = 17 AND bKind = 250;

INSERT TNPCITEMCHART (wNpcID, dwItemID)
VALUES (26011,25004), (26011,25009), (26011,25010), (26011,25013), (26011,25014);

COMMIT;
