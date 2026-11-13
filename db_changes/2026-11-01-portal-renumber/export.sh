#!/bin/sh
# usage: export.sh DBNAME OUTDIR : read-only dump of the chart rows the validator compares
DB=$1; O=$2; mkdir -p "$O"
q(){ sqlcmd -S "localhost\FOURSTORY" -E -C -d "$DB" -W -h -1 -s "|" -Q "SET NOCOUNT ON; $2" -o "$O/$1.txt" -f 65001; }
q portals "SELECT p.wPortalID,p.bCountry,p.wLocalID,p.wSpawnID,p.bCondition,s.wMapID,s.fPosX,s.fPosY,s.fPosZ FROM TPORTALCHART p LEFT JOIN TSPAWNPOSCHART s ON s.wID=p.wSpawnID ORDER BY 1"
q dests "SELECT wPortalID,wDestID,dwPrice,bEnable,bCondition1,dwConditionID1,bCondition2,dwConditionID2,bCondition3,dwConditionID3 FROM TDESTINATIONCHART ORDER BY 1,2"
q npcs "SELECT wID,bType,bCountryID,wItemID,wMapID,fPosX,fPosY,fPosZ,REPLACE(ISNULL(szName,''),'|','/') FROM TNPCCHART ORDER BY 1"
q npcitems "SELECT wNpcID,dwItemID FROM TNPCITEMCHART ORDER BY 1,2"
