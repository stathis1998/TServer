-- Three upgrade and reward items, 2026-11-24. Safe to run twice. Undo with undo.sql.
--   Hero's Medal (7927): using one adds wUseValue medals (OnCS_USEMEDALITEM_REQ). It was 0, now 10000.
--   Serendipity Potion (400%) (18045, was an unused copy of the 100% potion): new skill 948, a copy of
--     the 300% potion's skill 939 with value 400, so an upgrade gets its base chance plus 400% of it.
--   Epic Survival Tincture (18047, was an unused copy of the Survival Tincture): new skill 949, a copy of
--     the Greater tincture's skill 941 with exec 82 (SDT_STATUS_EPIC_ITEMGUARD). Only a TMapSvr built from
--     feature/upgrade-items knows exec 82; an older TMapSvr gives the buff but it protects nothing.
-- TMapSvr reads these charts at start, so restart it afterwards. Players need the client charts from
-- "TClient Upgrade Items.zip" (or a later zip) for the names, tooltips and buff icons.
SET XACT_ABORT ON;
BEGIN TRAN;

-- Hero's Medal
UPDATE dbo.TITEMCHART SET wUseValue = 10000
WHERE wItemID = 7927 AND bType = 15 AND bKind = 118 AND wUseValue IN (0, 10000);
IF @@ROWCOUNT <> 1
BEGIN
    RAISERROR('Item 7927 is not the Hero''s Medal this script expects, nothing changed', 16, 1);
    ROLLBACK; RETURN;
END

-- Skills 948 and 949, copied from 939 and 941 with only the name changed
IF NOT EXISTS (SELECT 1 FROM dbo.TSKILLCHART WHERE wID IN (948, 949))
BEGIN
    INSERT INTO dbo.TSKILLCHART (wID, szName, wPrevActiveID, wParentSkillID, wItemID, wMaxRange, wMinRange, wPosture,
        dwConditionID, dwWeaponID, dwClassID, bKind, fPrice, dwUseMP, bUseMPType, dwUseHP, bUseHPType, dwReuseDelay,
        nReuseDelayInc, dwLoopDelay, dwActionTime, dwDuration, dwDurationInc, dwKindDelay, dwAggro, dwAggroInc, bLevel,
        bMaxLevel, bNextLevel, bTarget, bTargetRange, bIsuse, bTargetHit, bPositive, bPriority, bSpeedApply, bCanLearn,
        bORadius, bIsRide, bIsDismount, wTargetActiveID, bMaintainType, bDuraSlot, bCanCancel, bHitTest, bHitInit,
        bHitInc, bGlobal, bRadius, bStatic, bEraseAct, bEraseHide, bIsHideSkill, bRunFromServer, bCheckAttacker,
        wTriggerID, wMapID, bRepeatCount)
    SELECT CASE wID WHEN 939 THEN 948 ELSE 949 END,
        CASE wID WHEN 939 THEN 'Serendipity Potion (400%)' ELSE 'Epic Survival Tincture' END,
        wPrevActiveID, wParentSkillID, wItemID, wMaxRange, wMinRange, wPosture,
        dwConditionID, dwWeaponID, dwClassID, bKind, fPrice, dwUseMP, bUseMPType, dwUseHP, bUseHPType, dwReuseDelay,
        nReuseDelayInc, dwLoopDelay, dwActionTime, dwDuration, dwDurationInc, dwKindDelay, dwAggro, dwAggroInc, bLevel,
        bMaxLevel, bNextLevel, bTarget, bTargetRange, bIsuse, bTargetHit, bPositive, bPriority, bSpeedApply, bCanLearn,
        bORadius, bIsRide, bIsDismount, wTargetActiveID, bMaintainType, bDuraSlot, bCanCancel, bHitTest, bHitInit,
        bHitInc, bGlobal, bRadius, bStatic, bEraseAct, bEraseHide, bIsHideSkill, bRunFromServer, bCheckAttacker,
        wTriggerID, wMapID, bRepeatCount
    FROM dbo.TSKILLCHART WHERE wID IN (939, 941);
    IF @@ROWCOUNT <> 2
    BEGIN
        RAISERROR('Skills 939 and 941 are not both there to copy, nothing changed', 16, 1);
        ROLLBACK; RETURN;
    END
END
ELSE IF (SELECT COUNT(*) FROM dbo.TSKILLCHART WHERE wID = 948 AND szName = 'Serendipity Potion (400%)')
      + (SELECT COUNT(*) FROM dbo.TSKILLCHART WHERE wID = 949 AND szName = 'Epic Survival Tincture') <> 2
BEGIN
    RAISERROR('Skill 948 or 949 is already used by something else, nothing changed', 16, 1);
    ROLLBACK; RETURN;
END

-- What the skills do: exec 42 is the Serendipity chance, exec 82 the Epic tincture
DELETE FROM dbo.TSKILLDATA WHERE wSkillID IN (948, 949);
INSERT INTO dbo.TSKILLDATA (wSkillID, bAction, bType, bAttr, bExec, bInc, wValue, wValueInc, bCalc) VALUES
    (948, 3, 6, 0, 42, 1, 400, 0, 0),
    (949, 3, 6, 0, 82, 1, 100, 0, 0);

-- The two items, made like the 300% potion (7863) and the Greater tincture (7921)
UPDATE dbo.TITEMCHART SET szNAME = 'Serendipity Potion (400%)', wUseValue = 948, fPrice = 10000000, bIsSell = 5,
    bDropLevel = 0, bIsSpecial = 1, bGroupID = 1, dwCode = 1048783
WHERE wItemID = 18045 AND bType = 7 AND bKind = 33 AND wUseValue IN (905, 948);
IF @@ROWCOUNT <> 1
BEGIN
    RAISERROR('Item 18045 is not the unused Serendipity Potion this script expects, nothing changed', 16, 1);
    ROLLBACK; RETURN;
END

UPDATE dbo.TITEMCHART SET szNAME = 'Epic Survival Tincture', wUseValue = 949, fPrice = 40000000, bIsSell = 4,
    bDropLevel = 0, bIsSpecial = 1, bGroupID = 1, dwCode = 1048783
WHERE wItemID = 18047 AND bType = 7 AND bKind = 33 AND wUseValue IN (919, 949);
IF @@ROWCOUNT <> 1
BEGIN
    RAISERROR('Item 18047 is not the unused Survival Tincture this script expects, nothing changed', 16, 1);
    ROLLBACK; RETURN;
END

COMMIT;
SELECT wItemID, bType, bKind, szNAME, wUseValue, fPrice FROM dbo.TITEMCHART WHERE wItemID IN (7927, 18045, 18047) ORDER BY wItemID;
SELECT c.wID, c.szName, d.bExec, d.wValue FROM dbo.TSKILLCHART c JOIN dbo.TSKILLDATA d ON d.wSkillID = c.wID WHERE c.wID IN (948, 949) ORDER BY c.wID;
